/*=============================================================================
 * Copyright (c) 2019, Martin Ribelotta <martinribelotta@gmail.com>
 * All rights reserved.
 * License: bsd-2-clause (see LICENSE.txt)
 * Date: 2019/10/27
 * Version: 0.1
 *===========================================================================*/

#include <sapi.h>
#include <editline.h>
#include <stdio.h>
#include <string.h>

#include <thumb.h>

static void putstr(const char *str) { uartWriteString(UART_USB, str); }

static void promt() {
   uartWriteString(UART_USB, "$> ");
}

struct thumb_disasm_t dis;

static void doDissasemble(uint32_t addr, size_t size)
{
   uint32_t stop = addr+size;
   printf("Dissasembling from 0x%08X to 0x%08X (size: %d bytes)\r\n", addr, stop, size);
   while (addr <  stop) {
      dis.pc = addr;
      dis.buf = (uint8_t*) addr;
      int len = thumb_disasm (&dis);
      if (len == 2)
         printf ("0x%08x %04x      %s\r\n", dis.pc, *(uint16_t*) dis.buf, dis.str);
      else
         printf ("0x%08x %04x %04x %s\r\n", dis.pc, *(uint16_t*) dis.buf, *(((uint16_t*)dis.buf) + 1), dis.str);
      addr += len;
   }
}

static uint32_t disasmIp = -1;
static char cmd[10];

static void execLine(char* l)
{
   uint32_t address, size;
   if (sscanf(l, "%9s", cmd) != 1) {
      promt();
      return;
   }
   if (strcmp(cmd, "dis") == 0) {
      size_t size = 40;
      uint32_t a, b;
      switch (sscanf(l + 4, "%i %i", &a, &b)) {
      case 1:
         size = a;
         break;
      case 2:
         disasmIp = a;
         size = b;
         break;
      }
      doDissasemble(disasmIp, size);
      disasmIp += size;
      disasmIp = (disasmIp + (4 - 1)) & -4;
   } else if (strcmp(cmd, "ip") == 0) {
      uint32_t addr;
      if (sscanf(l + 3, "%i", &addr) == 2) {
         printf("Set IP = 0x%08X\r\n", addr);
         disasmIp = addr;
      } else {
         printf("IP = 0x%08X\r\n", disasmIp);
      }
   } else
      printf("unknow command: %s\r\nTry: dis, ip\r\n", cmd);
   promt();
}

static lineEditor_t ed;
static char buf[128];

int main( void )
{
   boardInit();
   editLineInit(&ed, buf, sizeof(buf), putstr, execLine);
   disasmIp = (*(uint32_t**)0xE000ED08)[1] & ~1U;
   printf("Welcome to thumb2 dissasembler v0.1\r\n"
          "Copyright (c) 2019, Martin Ribelotta <martinribelotta@gmail.com>\r\n"
          "All rights reserved.\r\n"
          "License: bsd-2-clause (see LICENSE.txt)\r\n\r\n"
          "Usage:\r\n"
          "  for set instruction pointer use the commando \"ip <addr>\"\r\n"
          "  for view current IP use the commando \"ip\"\r\n"
          "  for start disassemble use \"dis <addr> <size>\"\r\n"
          "  if addr or size is ommited use current ip as addr and 40 as size\r\n\r\n"
   );
   promt();
   while( true ) {
      uint8_t c;
      if (uartReadByte(UART_USB, &c)) {
         editLineProcess(&ed, c);
      }
   }
   return 0;
}
