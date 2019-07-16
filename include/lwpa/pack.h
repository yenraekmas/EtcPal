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

/* lwpa/pack.h: Platform- and endianness-independent buffer packing and unpacking. */
#ifndef _LWPA_PACK_H_
#define _LWPA_PACK_H_

#include "lwpa/int.h"
#include "lwpa/opts.h"

/*! \defgroup lwpa_pack lwpa_pack
 *  \ingroup lwpa
 *  \brief Platform- and endianness-independent buffer packing and unpacking.
 *
 *  \#include "lwpa/pack.h"
 *
 *  This module defines macros for packing and unpacking integer types to/from a byte buffer. They
 *  are architected in such a way that the endianness of the integer in the buffer is always known,
 *  regardless of the endianness of your platform.
 *
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

uint16_t lwpa_upack_16b(const uint8_t* buf);
void lwpa_pack_16b(uint8_t* buf, uint16_t val);
uint16_t lwpa_upack_16l(const uint8_t* buf);
void lwpa_pack_16l(uint8_t* buf, uint16_t val);
uint32_t lwpa_upack_32b(const uint8_t* buf);
void lwpa_pack_32b(uint8_t* buf, uint32_t val);
uint32_t lwpa_upack_32l(const uint8_t* buf);
void lwpa_pack_32l(uint8_t* buf, uint32_t val);

#if LWPA_64BIT_SUPPORT
uint64_t lwpa_upack_64b(const uint8_t* buf);
void lwpa_pack_64b(uint8_t* buf, uint64_t val);
uint64_t lwpa_upack_64l(const uint8_t* buf);
void lwpa_pack_64l(uint8_t* buf, uint64_t val);
#endif

#ifdef __cplusplus
}
#endif

/*! @} */

#endif /* _LWPA_PACK_H_ */