/*=============================================================================
 * Copyright (c) 2019, Martin Ribelotta <martinribelotta@gmail.com>
 * All rights reserved.
 * License: bsd-2-clause (see LICENSE.txt)
 * Date: 2019/10/06
 * Version: 0.1
 *===========================================================================*/

#include <editline.h>

#include <ctype.h>

void editLineInit(lineEditor_t *ed, char *buf, size_t size, void (*put)(const char*), void (*executor)(char*))
{
   ed->lineBuf = buf;
   ed->lineBufSize = size;
   ed->lineBufIdx = 0;
   ed->putstr = put;
   ed->execute = executor;
}

void editLineProcess(lineEditor_t *ed, int c)
{
   switch(c) {
   case '\r':
      ed->putstr("\r\n");
      ed->lineBuf[ed->lineBufIdx] = 0;
      ed->execute(ed->lineBuf);
      ed->lineBufIdx = 0;
      break;
   case '\b':
      if (ed->lineBufIdx > 0) {
         ed->lineBuf[--(ed->lineBufIdx)] = 0;
         ed->putstr("\b \b");
      }
      break;
   default:
      if (isprint(c) && (ed->lineBufIdx < (ed->lineBufSize - 1))) {
         ed->lineBuf[ed->lineBufIdx++] = c;
         ed->lineBuf[ed->lineBufIdx] = 0;
         char buf[] = { c, 0 };
         ed->putstr(buf);
      }
      break;
   }
}
