/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/04/26
 * Version: 1
 *===========================================================================*/

#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#ifdef __cplusplus
extern "C" {
#endif

void fsmSemaphoreInit( void );
void fsmSemaphoreUpdate( void );

#ifdef __cplusplus
}
#endif

#endif /* __SEMAPHORE_H__ */
