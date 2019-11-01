/*
 * Using code from https://github.com/radare/pebble-disthumb
 */

/* 2015 - tiny Thumb2 disassembler based on winedbg code */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thumb.h"

#define ARM_INSN_SIZE    4
#define THUMB_INSN_SIZE  2
#define THUMB2_INSN_SIZE 4

#define ROR32(n, r) (((n) >> (r)) | ((n) << (32 - (r))))

#define get_cond(ins)           tbl_cond[(ins >> 28) & 0x0f]
#define get_nibble(ins, num)    ((ins >> (num * 4)) & 0x0f)

struct inst_thumb16 {
	uint16_t mask;
	uint16_t pattern;
	uint16_t (*func)(struct thumb_disasm_t*, uint16_t);
};

struct inst_arm {
	uint32_t mask;
	uint32_t pattern;
	uint32_t (*func)(struct thumb_disasm_t*, uint32_t);
};

static char const tbl_regs[][4] = {
	"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10",
	"fp", "ip", "sp", "lr", "pc", "cpsr"
};

static char const tbl_cond[][3] = {
	"eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge",
	"lt", "gt", "le", "", ""
};

static char const tbl_shifts[][4] = {
	"lsl", "lsr", "asr", "ror"
};

static char const tbl_hiops_t[][4] = {
	"add", "cmp", "mov", "bx"
};

static char const tbl_aluops_t[][4] = {
	"and", "eor", "lsl", "lsr", "asr", "adc", "sbc", "ror", "tst", "neg",
	"cmp", "cmn", "orr", "mul", "bic", "mvn"
};

static char const tbl_immops_t[][4] = {
	"mov", "cmp", "add", "sub"
};

static char const tbl_sregops_t[][5] = {
	"strh", "ldsb", "ldrh", "ldsh"
};

static uint32_t db_get_inst(const uint8_t* buf, int size) {
	if (size == 4) return *(uint32_t*)buf;
	if (size == 2) return (uint32_t)*(uint16_t*)buf;
	return 0;
}

static uint16_t thumb_disasm_hireg(struct thumb_disasm_t *ai, uint16_t inst) {
	short dst = inst & 0x07;
	short src = (inst >> 3) & 0x07;
	short h2  = (inst >> 6) & 0x01;
	short h1  = (inst >> 7) & 0x01;
	short op  = (inst >> 8) & 0x03;

	if (h1) dst += 8;
	if (h2) src += 8;

	if (op == 2 && dst == src) { /* mov rx, rx */
		strcpy (ai->str, "nop");
		return 0;
	}

	if (op == 3)
		snprintf (ai->str, sizeof (ai->str), "b%sx %s", h1?"l":"", tbl_regs[src]);
	else
		snprintf (ai->str, sizeof (ai->str), "%s %s, %s", tbl_hiops_t[op], tbl_regs[dst], tbl_regs[src]);

	return 0;
}

static uint16_t thumb_disasm_aluop(struct thumb_disasm_t *ai, uint16_t inst) {
	short dst = inst & 0x07;
	short src = (inst >> 3) & 0x07;
	short op  = (inst >> 6) & 0x0f;
	snprintf (ai->str, sizeof (ai->str),
		"%s %s, %s", tbl_aluops_t[op], tbl_regs[dst], tbl_regs[src]);
	return 0;
}

// XXX possible overflow here? resize str plz
static uint16_t thumb_disasm_pushpop(struct thumb_disasm_t *ai, uint16_t inst) {
	short lrpc = (inst >> 8)  & 0x01;
	short load = (inst >> 11) & 0x01;
	short i;
	short last;

	for (i=7;i>=0;i--)
		if ((inst>>i) & 1) break;
	last = i;

	strcpy (ai->str, load? "pop {": "push {");

	for (i=0;i<=7;i++) {
		if ((inst>>i) & 1) {
			if (i == last) strcat (ai->str, tbl_regs[i]); // overflow ?? 
			else strcat (ai->str, tbl_regs[i]);
		}
	}
	if (lrpc) {
		if (last) strcat (ai->str, ", ");
		strcat (ai->str, load? "pc": "lr");
	}
	
	strcat (ai->str, "}");
	return 0;
}

static uint16_t thumb_disasm_blocktrans(struct thumb_disasm_t *ai, uint16_t inst) {
	short load = (inst >> 11) & 0x01;
	short i;
	short last;

	for (i=7;i>=0;i--)
		if ((inst>>i) & 1) break;
	last = i;

	snprintf (ai->str, sizeof (ai->str),
		"%s %s!, {", load ? "ldmia" : "stmia", tbl_regs[(inst >> 8) & 0x07]);

	for (i=0;i<=7;i++) {
		if ((inst>>i) & 1) {
			strcat (ai->str, tbl_regs[i]);
			if (i != last) {
				strcat (ai->str, ", ");
			}
		}
	}

	strcat (ai->str, "}");
	return 0;
}

static uint16_t thumb_disasm_condbranch(struct thumb_disasm_t *ai, uint16_t inst) {
	uint16_t offset = inst & 0x00ff;
	snprintf (ai->str, sizeof (ai->str),
		"b%s 0x%lx", tbl_cond[(inst >> 8) & 0x0f], ai->pc+offset);
	ai->jmp = ai->pc + offset;
	ai->fail = ai->pc + 4;
	return 0;
}

static uint16_t thumb_disasm_uncondbranch(struct thumb_disasm_t *ai, uint16_t inst) {
	short offset = (inst & 0x07ff) << 1;
	if (offset & 0x0800) offset |= 0xf000;
	offset += 4;
	snprintf (ai->str, sizeof (ai->str), "b 0x%lx", ai->pc+offset);
	ai->jmp = ai->pc+offset;
	return 0;
}

static uint16_t thumb_disasm_loadadr(struct thumb_disasm_t *ai, uint16_t inst) {
	uint16_t src = (inst >> 11) & 0x01;
	uint16_t offset = (inst & 0xff) << 2;
	snprintf (ai->str, sizeof (ai->str),
		"add %s, %s, #%d", tbl_regs[(inst >> 8) & 0x07], src ? "sp" : "pc",
			offset);
	return 0;
}

static uint16_t thumb_disasm_swi(struct thumb_disasm_t *ai, uint16_t inst) {
	uint16_t comment = inst & 0x00ff;
	snprintf (ai->str, sizeof (ai->str), "swi #%d", comment);
	return 0;
}

static uint16_t thumb_disasm_nop(struct thumb_disasm_t *ai, uint16_t inst) {
	strcpy (ai->str, "nop");
	return 0;
}

static uint16_t thumb_disasm_ldrpcrel(struct thumb_disasm_t *ai, uint16_t inst) {
	uint16_t offset = (inst & 0xff) << 2;
	snprintf (ai->str, sizeof (ai->str),
		"ldr %s, [pc, #%u]", tbl_regs[(inst >> 8) & 0x07], offset);
	return 0;
}

static uint16_t thumb_disasm_ldrsprel(struct thumb_disasm_t *ai, uint16_t inst) {
	uint16_t offset = (inst & 0xff) << 2;
	snprintf (ai->str, sizeof (ai->str),
		"%s %s, [sp, #%u]", (inst & 0x0800)?"ldr":"str",
		tbl_regs[(inst >> 8) & 0x07], offset);
	return 0;
}

static uint16_t thumb_disasm_addsprel(struct thumb_disasm_t *ai, uint16_t inst) {
	uint16_t offset = (inst & 0x7f) << 2;
	int sub = ((inst >> 7) & 0x01);
	snprintf (ai->str, sizeof (ai->str),
		"%s sp, sp, #%u", sub? "sub": "add", offset);
	return 0;
}

static uint16_t thumb_disasm_ldrimm(struct thumb_disasm_t *ai, uint16_t inst) {
	uint16_t offset = (inst & 0x07c0) >> 6;
	snprintf (ai->str, sizeof (ai->str), "%s%s %s, [%s, #%u]",
			(inst & 0x0800)?"ldr":"str", (inst & 0x1000)?"b":"",
			tbl_regs[inst & 0x07], tbl_regs[(inst >> 3) & 0x07],
			(inst & 0x1000)?offset:(offset << 2));
	return 0;
}

static uint16_t thumb_disasm_ldrhimm(struct thumb_disasm_t *ai, uint16_t inst) {
	uint16_t offset = (inst & 0x07c0) >> 5;
	snprintf (ai->str, sizeof (ai->str), "%s %s, [%s, #%u]", (inst & 0x0800)?"ldrh":"strh",
			tbl_regs[inst & 0x07], tbl_regs[(inst >> 3) & 0x07], offset);
	return 0;
}

static uint16_t thumb_disasm_ldrreg(struct thumb_disasm_t *ai, uint16_t inst) {
	snprintf (ai->str, sizeof (ai->str), "%s%s %s, [%s, %s]",
			(inst & 0x0800)?"ldr":"str", (inst & 0x0400)?"b":"",
			tbl_regs[inst & 0x07], tbl_regs[(inst >> 3) & 0x07],
			tbl_regs[(inst >> 6) & 0x07]);
	return 0;
}

static uint16_t thumb_disasm_ldrsreg(struct thumb_disasm_t *ai, uint16_t inst) {
	snprintf (ai->str, sizeof (ai->str), "%s %s, [%s, %s]",
			tbl_sregops_t[(inst >> 10) & 0x03], tbl_regs[inst & 0x07],
			tbl_regs[(inst >> 3) & 0x07], tbl_regs[(inst >> 6) & 0x07]);
	return 0;
}

static uint16_t thumb_disasm_immop(struct thumb_disasm_t *ai, uint16_t inst) {
	uint16_t op = (inst >> 11) & 0x03;
	snprintf (ai->str, sizeof (ai->str),
		"%s %s, #%u", tbl_immops_t[op], tbl_regs[(inst >> 8) & 0x07], inst & 0xff);
	return 0;
}

static uint16_t thumb_disasm_addsub(struct thumb_disasm_t *ai, uint16_t inst) {
	char last [32];
	uint16_t op = (inst >> 9) & 0x01;
	uint16_t immediate = (inst >> 10) & 0x01;
	if (immediate) snprintf (last, sizeof (last), "#%d", (inst >> 6) & 0x07);
	else strcpy (last, tbl_regs[(inst >> 6) & 0x07]);
	snprintf (ai->str, sizeof (ai->str),
		"%s %s, %s, %s", op ? "sub" : "add",
		tbl_regs[inst & 0x07], tbl_regs[(inst >> 3) & 0x07], last);
	return 0;
}

static uint16_t thumb_disasm_movshift(struct thumb_disasm_t *ai, uint16_t inst) {
	uint16_t op = (inst >> 11) & 0x03;
	snprintf (ai->str, sizeof (ai->str),
		"%s %s, %s, #%u", tbl_shifts[op],
		tbl_regs[inst & 0x07], tbl_regs[(inst >> 3) & 0x07], (inst >> 6) & 0x1f);
	return 0;
}

static uint32_t getslice(int msb, int lsb, uint32_t v) {
   uint32_t mask = (1 << (msb - lsb + 1)) - 1;
   v >>= lsb;
   return v & mask;
}

static uint32_t getbit(int bit, uint32_t v)
{
   return getslice(bit, bit, v);
}

static uint32_t thumbExpandImm(int i, int imm3, int imm8)
{
   uint32_t imm12 = imm8 | (imm3 << 8) | (i << (8 + 3));
   // TODO incorrect implementation
   return imm12;
}

static uint32_t thumb2_disasm_dataproc(struct thumb_disasm_t *ai, uint32_t inst) {
   uint16_t insth = (inst >> 16) & 0xffff;
   uint16_t instl = inst & 0xffff;
   int nIsMod12BitImm = getbit(9, insth);
   int i = getbit(10, insth);
   int rn = getslice(3, 0, insth);
   int imm3 = getslice(14, 12, instl);
   int rd = getslice(11, 8, instl);
   int imm8 = getslice(7, 0, instl);
   uint32_t imm32 = thumbExpandImm(i, imm3, imm8);
   if (nIsMod12BitImm == 0) {
      int s = getbit(4, insth);
      int op = getslice(8, 5, insth);
      const char *opcode = "???";
      switch(op) {
      case 0b1010 : // Add with carry ADC (immediate)
         opcode = s? "adc":"adcs";
         break;
      case 0b1000 : // Add ADD (immediate)
                    // Compare negative CMN (immediate) " ADD with Rd == 0b1111, S == 1"
         opcode = s? (rd == 0xf)? "cmn":"adds":"add";
         break;
      case 0b0000 : // Logical AND AND (immediate)
                    // Test TST (immediate) " AND with Rd == 0b1111, S == 1"
         opcode = s? (rd==0xf)? "tst":"ands":"and";
         break;
      case 0b0001 : // Bit clear BIC (immediate)
         opcode = "bic";
         break;
      case 0b1101 : // Compare CMP (immediate) " SUB with Rd == 0b1111, S == 1"
                    // Subtract SUB (immediate)
         opcode = s? (rd==0xf)? "cmp":"subs":"sub";
         break;
      case 0b0100 : // Exclusive OR EOR (immediate)
                    // Test equal TEQ (immediate) " EOR with Rd == 0b1111, S == 1"
         opcode = s? (rd==0xf)? "teq":"eors":"eor";
         break;
      case 0b0010 : // Move MOV (immediate) " ORR with Rn == 0b1111"
                    // Logical OR ORR (immediate)
         opcode = s? (rd==0xf)? "mov":"orrs":"orr";
         break;
      case 0b0011 : // Move negative MVN (immediate) " ORN with Rn == 0b1111"
                    // Logical OR NOT ORN (immediate)
         opcode = s? (rd==0xf)? "mvn":"orns":"orn";
         break;
      case 0b1110 : // Reverse subtract RSB (immediate)
         opcode = "rsb";
         break;
      case 0b1011 : // Subtract with carry SBC (immediate)
         opcode = "sbc";
         break;
      }
      snprintf(ai->str, sizeof (ai->str),
               "%s.w %s, %s, #%u",
               opcode, tbl_regs[rd], tbl_regs[rn], imm32);
   } else {
      int isBfieldOp = getbit(8, insth);
      if (isBfieldOp == 1) {
         int op = getslice(7, 5, insth);
         int sbz = i;
         int imm2 = getslice(7, 6, instl);
         int sbz2 = getbit(5, instl);
         int imm5 = getslice(4, 0, instl);
         // TODO
      } else {
         int op = getbit(7, insth);
         int op2 = getslice(5, 4, insth);
         int isPlain16b = getbit(6, insth);
         if (isPlain16b) {
            int imm4 = rn;
            uint32_t imm32p = (imm4 << (8+3+1)) | (i<<(8+3)) | (imm3<<8) | imm8;
            snprintf(ai->str, sizeof (ai->str),
                     "%s.w %s, #%u",
                     op? "movt":"mov", tbl_regs[rd], imm32);
         } else {
            if (op) {
               snprintf(ai->str, sizeof (ai->str),
                        "%s.w %s, %s, #%u",
                        (op2 == 0)? "add": (op2 == 2)? "sub":"undef",
                        tbl_regs[rd], tbl_regs[rn], imm32);
            } else {
               // TODO
            }
         }
      }
   }
   return 0;
}

static uint32_t thumb2_disasm_branchlinked(struct thumb_disasm_t *ai, uint32_t inst) {
	uint32_t offset = (((inst & 0x07ff0000) >> 4) | ((inst & 0x000007ff) << 1)) + 4;
	snprintf (ai->str, sizeof (ai->str), "bl 0x%lx", ai->pc+offset);
	ai->jmp = ai->pc+offset;
	return 0;
}

static uint32_t thumb2_disasm_misc(struct thumb_disasm_t *ai, uint32_t inst) {
	uint16_t op1 = (inst >> 20) & 0x03;
	uint16_t op2 = (inst >> 4) & 0x03;

	if (get_nibble(inst, 4) != get_nibble(inst, 0))
		return inst;

	if (op1 == 3 && op2 == 0) {
		snprintf (ai->str, sizeof (ai->str),
			"clz %s, %s ", tbl_regs[get_nibble(inst, 2)],
			tbl_regs[get_nibble(inst, 0)]);
		return 0;
	}

	if (op1 == 1) {
		const char *op = "";
		switch (op2) {
		case 0: op = "rev "; break;
		case 1: op = "rev16 "; break;
		case 2: op = "rbit "; break;
		case 3: op = "revsh "; break;
		}
		snprintf (ai->str, sizeof (ai->str),
			"%s %s, %s ", op, tbl_regs[get_nibble(inst, 2)], tbl_regs[get_nibble(inst, 0)]);
		return 0;
	}

	return inst;
}

static uint32_t thumb2_disasm_mul(struct thumb_disasm_t *ai, uint32_t inst) {
	uint16_t op1 = (inst >> 20) & 0x07;
	uint16_t op2 = (inst >> 4) & 0x03;

	if (op1)
		return inst;

	if (op2 == 0) {
		uint16_t nib = get_nibble (inst, 3);
		if (nib == 0xf) {
			snprintf (ai->str, sizeof (ai->str),
				"mul %s, %s, %s ", tbl_regs[get_nibble(inst, 2)],
				tbl_regs[get_nibble(inst, 4)],
				tbl_regs[get_nibble(inst, 0)]);
		} else {
			snprintf (ai->str, sizeof (ai->str),
				"mla %s, %s, %s, %s ", tbl_regs[get_nibble(inst, 2)],
				tbl_regs[get_nibble(inst, 4)],
				tbl_regs[get_nibble(inst, 0)],
				tbl_regs[get_nibble(inst, 3)]);
		}
		return 0;
	}

	if (op2 == 1) {
		snprintf (ai->str, sizeof (ai->str),
			"mls %s, %s, %s, %s ", tbl_regs[get_nibble(inst, 2)],
			tbl_regs[get_nibble(inst, 4)],
			tbl_regs[get_nibble(inst, 0)],
			tbl_regs[get_nibble(inst, 3)]);
		return 0;
	}

	return inst;
}

static uint32_t thumb2_disasm_longmuldiv(struct thumb_disasm_t *ai, uint32_t inst) {
	const char *op = "";
	uint16_t op1 = (inst >> 20) & 0x07;
	uint16_t op2 = (inst >> 4) & 0x0f;

	if (op2 == 0) {
		switch (op1) {
		case 0: op = "smull "; break;
		case 2: op = "umull "; break;
		case 4: op = "smlal "; break;
		case 6: op = "umlal "; break;
		default: return inst;
		}
		snprintf (ai->str, sizeof (ai->str), "%s%s, %s, %s, %s ", op,
			tbl_regs[get_nibble(inst, 3)], tbl_regs[get_nibble(inst, 2)],
			tbl_regs[get_nibble(inst, 4)], tbl_regs[get_nibble(inst, 0)]);
		return 0;
	}

	if (op2 == 0xffff) {
		switch (op1) {
		case 1: op = "sdiv "; break;
		case 3: op = "udiv "; break;
		default: return inst;
		}
		snprintf (ai->str, sizeof (ai->str), "%s%s, %s, %s ", op,
			tbl_regs[get_nibble(inst, 2)], tbl_regs[get_nibble(inst, 4)],
			tbl_regs[get_nibble(inst, 0)]);
		return 0;
	}
	return inst;
}

static uint32_t thumb2_disasm_coprocmov1(struct thumb_disasm_t *ai, uint32_t inst) {
	uint16_t opc1 = (inst >> 21) & 0x07;
	uint16_t opc2 = (inst >> 5) & 0x07;
	char last[32];
	if (opc2) snprintf (last, sizeof (last), ", #%u", opc2); else *last = 0;
	snprintf (ai->str, sizeof (ai->str),
		"%s%s  p%lu, #%u, %s, cr%lu, cr%lu%s", (inst & 0x00100000)?"mrc":"mcr",
		(inst & 0x10000000)?"2":"", get_nibble(inst, 2), opc1,
		tbl_regs[get_nibble(inst, 3)], get_nibble(inst, 4), get_nibble(inst, 0), last);
	return 0;
}

static const struct inst_thumb16 tbl_thumb16[] = {
	{ 0xfc00, 0x4400, thumb_disasm_hireg },
	{ 0xfc00, 0x4000, thumb_disasm_aluop },
	{ 0xf600, 0xb400, thumb_disasm_pushpop },
	{ 0xf000, 0xc000, thumb_disasm_blocktrans },
	{ 0xf000, 0xd000, thumb_disasm_condbranch },
	{ 0xf800, 0xe000, thumb_disasm_uncondbranch },
	{ 0xf000, 0xa000, thumb_disasm_loadadr },
	{ 0xf800, 0x4800, thumb_disasm_ldrpcrel },
	{ 0xf000, 0x9000, thumb_disasm_ldrsprel },
	{ 0xff00, 0xb000, thumb_disasm_addsprel },
	{ 0xe000, 0x6000, thumb_disasm_ldrimm },
	{ 0xf000, 0x8000, thumb_disasm_ldrhimm },
	{ 0xf200, 0x5000, thumb_disasm_ldrreg },
	{ 0xf200, 0x5200, thumb_disasm_ldrsreg },
	{ 0xe000, 0x2000, thumb_disasm_immop },
	{ 0xff00, 0xdf00, thumb_disasm_swi },
	{ 0xff00, 0xbf00, thumb_disasm_nop },
	{ 0xf800, 0x1800, thumb_disasm_addsub },
	{ 0xe000, 0x0000, thumb_disasm_movshift },
	{ 0x0000, 0x0000, NULL }
};

static const struct inst_arm tbl_thumb32[] = {
	{ 0xf8008000, 0xf0000000, thumb2_disasm_dataproc },
	{ 0xf800f800, 0xf000f800, thumb2_disasm_branchlinked },
	{ 0xffc0f0c0, 0xfa80f080, thumb2_disasm_misc },
	{ 0xff8000c0, 0xfb000000, thumb2_disasm_mul },
	{ 0xff8000f0, 0xfb800000, thumb2_disasm_longmuldiv },
	{ 0xff8000f0, 0xfb8000f0, thumb2_disasm_longmuldiv },
	{ 0xef100010, 0xee100010, thumb2_disasm_coprocmov1 },
	{ 0xef100010, 0xee000010, thumb2_disasm_coprocmov1 },
	{ 0x00000000, 0x00000000, NULL }
};

int thumb_disasm(struct thumb_disasm_t *ai) {
	struct inst_thumb16 *t_ptr = (struct inst_thumb16 *)&tbl_thumb16;
	struct inst_arm *t2_ptr = (struct inst_arm *)&tbl_thumb32;
	uint32_t inst;
	uint16_t tinst;
	int size;
	int matched = 0;

	ai->jmp = ai->fail = UT32_MAX;

	tinst = db_get_inst(ai->buf, THUMB_INSN_SIZE);
	switch (tinst & 0xf800) {
	case 0xe800:
	case 0xf000:
	case 0xf800:
		size = THUMB2_INSN_SIZE;
		inst = db_get_inst(ai->buf + 2, THUMB_INSN_SIZE);
		inst |= (tinst << 16);

		while (t2_ptr->func) {
			if ((inst & t2_ptr->mask) ==  t2_ptr->pattern) {
				matched = 1;
				break;
			}
			t2_ptr++;
		}
		if (!matched) strcpy (ai->str, "invalid"); // thumb2
		else t2_ptr->func (ai, inst);
		snprintf (ai->hex, sizeof (ai->hex), "%02x%02x%02x%02x",
			*((uint8_t*)(&inst)), *((uint8_t*)(&inst)+1),
			*((uint8_t*)(&inst)+2),  *((uint8_t*)(&inst)+3));
		return size;
	default:
		break;
	}

	size = THUMB_INSN_SIZE;
	while (t_ptr->func) {
		if ((tinst & t_ptr->mask) ==  t_ptr->pattern) {
			matched = 1;
			break;
		}
		t_ptr++;
	}

	if (!matched) snprintf (ai->str, sizeof (ai->str), "0x%04x", tinst);
	else t_ptr->func(ai, tinst);
	snprintf (ai->hex, sizeof (ai->hex), "%02x%02x", *((uint8_t*)(&tinst)), *((uint8_t*)(&tinst)+1));
	return size;
}

#if 0
int main() {
	int len;
	struct thumb_disasm_t dis = {
		.pc = 0x100,
		.buf = (const uint8_t*)"\x59\x46"
	};
	len = thumb_disasm (&dis);
	printf ("0x%04x  %d  %s\n", dis.pc, len, dis.str);
	return 0;
}
#endif
