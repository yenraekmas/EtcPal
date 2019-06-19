/******************************************************************************
 * Copyright 2019 ETC Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************
 * This file is a part of lwpa. For more information, go to:
 * https://github.com/ETCLabs/lwpa
 ******************************************************************************/

#ifndef _LWPA_OS_LOCK_H_
#define _LWPA_OS_LOCK_H_

#include <mqx.h>
#include <lwevent.h>
#include "lwpa/common.h"
#include "lwpa/bool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef LWSEM_STRUCT lwpa_mutex_t;

#define lwpa_mutex_create(idptr) (MQX_OK == _lwsem_create((idptr), 1))
#define lwpa_mutex_take(idptr) (MQX_OK == _lwsem_wait(idptr))
#define lwpa_mutex_try_take(idptr) (MQX_OK == _lwsem_wait_ticks((idptr), 1u))
#define lwpa_mutex_give(idptr) ((void)_lwsem_post(idptr))
#define lwpa_mutex_destroy(idptr) ((void)_lwsem_destroy(idptr))

typedef LWEVENT_STRUCT lwpa_signal_t;

#define lwpa_signal_create(idptr) (MQX_OK == _lwevent_create((idptr), LWEVENT_AUTO_CLEAR))
#define lwpa_signal_wait(idptr) (MQX_OK == _lwevent_wait_ticks((idptr), 1u, true, 0u))
#define lwpa_signal_poll(idptr) (MQX_OK == _lwevent_wait_ticks((idptr), 1u, true, 1u))
#define lwpa_signal_post(idptr) ((void)_lwevent_set((idptr), 1u))
#define lwpa_signal_destroy(idptr) ((void)_lwevent_destroy(idptr))

typedef struct
{
  bool valid;
  LWSEM_STRUCT sem;
  unsigned int reader_count;
} lwpa_rwlock_t;

bool lwpa_rwlock_create(lwpa_rwlock_t* id);
bool lwpa_rwlock_readlock(lwpa_rwlock_t* id);
bool lwpa_rwlock_try_readlock(lwpa_rwlock_t* id);
void lwpa_rwlock_readunlock(lwpa_rwlock_t* id);
bool lwpa_rwlock_writelock(lwpa_rwlock_t* id);
bool lwpa_rwlock_try_writelock(lwpa_rwlock_t* id);
void lwpa_rwlock_writeunlock(lwpa_rwlock_t* id);
void lwpa_rwlock_destroy(lwpa_rwlock_t* id);

#ifdef __cplusplus
}
#endif

#endif /* _LWPA_OS_LOCK_H_ */
