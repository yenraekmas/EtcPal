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
 * This file is a part of EtcPal. For more information, go to:
 * https://github.com/ETCLabs/EtcPal
 ******************************************************************************/

#include "etcpal/lock.h"

/**************************** Private constants ******************************/

#define MAX_READERS 20000

/*********************** Private function prototypes *************************/

static void reader_atomic_increment(etcpal_rwlock_t* id);
static void reader_atomic_decrement(etcpal_rwlock_t* id);

/*************************** Function definitions ****************************/

bool etcpal_rwlock_create(etcpal_rwlock_t* id)
{
  if (id && (MQX_OK == _lwsem_create(&id->sem, 1)))
  {
    id->valid = true;
    id->reader_count = 0;
    return true;
  }
  return false;
}

bool etcpal_rwlock_readlock(etcpal_rwlock_t* id)
{
  bool res = false;
  if (id && id->valid)
  {
    if (MQX_OK == _lwsem_wait(&id->sem))
    {
      if (id->reader_count < MAX_READERS)
      {
        // Add one to the reader count.
        reader_atomic_increment(id);
        res = true;
      }
      // Allow other readers to access.
      _lwsem_post(&id->sem);
    }
  }
  return res;
}

bool etcpal_rwlock_try_readlock(etcpal_rwlock_t* id)
{
  bool res = false;
  if (id && id->valid)
  {
    if (MQX_OK == _lwsem_wait_ticks(&id->sem, 1u))
    {
      if (id->reader_count < MAX_READERS)
      {
        reader_atomic_increment(id);
        res = true;
      }
      _lwsem_post(&id->sem);
    }
  }
  return res;
}

void etcpal_rwlock_readunlock(etcpal_rwlock_t* id)
{
  if (id && id->valid)
    reader_atomic_decrement(id);
}

bool etcpal_rwlock_writelock(etcpal_rwlock_t* id)
{
  if (id && id->valid)
  {
    if (MQX_OK == _lwsem_wait(&id->sem))
    {
      // Wait until there are no readers, keeping the lock so that no new readers can get in.
      while (id->reader_count > 0)
      {
        _time_delay(1);
      }
      // Hold on to the lock until writeunlock() is called
      return true;
    }
  }
  return false;
}

bool etcpal_rwlock_try_writelock(etcpal_rwlock_t* id)
{
  if (id && id->valid)
  {
    if (MQX_OK == _lwsem_wait_ticks(&id->sem, 1u))
    {
      // Just check once to see if there are still readers
      if (id->reader_count > 0)
      {
        // Readers are present, give up the lock and return false below.
        _lwsem_post(&id->sem);
      }
      else
      {
        // Return, holding the lock
        return true;
      }
    }
  }
  return false;
}

void etcpal_rwlock_writeunlock(etcpal_rwlock_t* id)
{
  if (id && id->valid)
    _lwsem_post(&id->sem);
}

void etcpal_rwlock_destroy(etcpal_rwlock_t* id)
{
  if (id && id->valid)
  {
    _lwsem_destroy(&id->sem);
    id->valid = false;
  }
}

void reader_atomic_increment(etcpal_rwlock_t* id)
{
  _int_disable();
  ++id->reader_count;
  _int_enable();
}

void reader_atomic_decrement(etcpal_rwlock_t* id)
{
  _int_disable();
  --id->reader_count;
  _int_enable();
}
