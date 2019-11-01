/*
 * Using code from https://github.com/radare/pebble-disthumb
 */
#ifndef _INCLUDE_THUMB_DISASM_H_
#define _INCLUDE_THUMB_DISASM_H_

#include <stdint.h>
#if 0
#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#endif
#define UT32_MAX 0xffffffff

struct thumb_disasm_t {
	uint32_t pc;
	const uint8_t *buf;
	char str[32];
	char hex[32];
	uint32_t jmp, fail;
};

int thumb_disasm(struct thumb_disasm_t *arminsn);

#endif