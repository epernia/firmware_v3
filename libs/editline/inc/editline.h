/*=============================================================================
 * Copyright (c) 2019, Martin Ribelotta <martinribelotta@gmail.com>
 * All rights reserved.
 * License: bsd-2-clause (see LICENSE.txt)
 * Date: 2019/10/06
 * Version: 0.1
 *===========================================================================*/

#ifndef __EDITLINE_H__
#define __EDITLINE_H__

#include <stddef.h>

typedef struct {
   char *lineBuf;
   int lineBufSize;
   int lineBufIdx;
   void (*putstr)(const char*);
   void (*execute)(char*);
} lineEditor_t;

extern void editLineInit(lineEditor_t *ed, char *buf, size_t size, void (*put)(const char*), void (*executor)(char*));
extern void editLineProcess(lineEditor_t *ed, int c);

#endif /* __EDITLINE_H__ */
