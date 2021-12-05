//
// Created by jngl on 05/12/2021.
//

#ifndef NUKLEARCPP_NUKLEAR_BASIC_H
#define NUKLEARCPP_NUKLEAR_BASIC_H

#include <cstdint>

using NK_INT8 = int8_t;
using NK_UINT8 = uint8_t;
using NK_INT16 = int16_t;
using NK_UINT16 = uint16_t;
using NK_INT32 = int32_t;
using NK_UINT32 = uint32_t;
using NK_SIZE_TYPE = uintptr_t;
using NK_POINTER_TYPE = uintptr_t;

using NK_BOOL = bool;

using nk_char = int8_t;
using nk_uchar = uint8_t;
using nk_byte = uint8_t;
using nk_short = int16_t;
using nk_ushort = uint16_t;
using nk_int = int32_t;
using nk_uint = uint32_t;
using nk_size = uintptr_t;
using nk_ptr = uintptr_t;
using nk_bool = bool;

using nk_hash = nk_uint;
using nk_flags = nk_uint;
using nk_rune = nk_uint;

/* Make sure correct type size:
 * This will fire with a negative subscript error if the type sizes
 * are set incorrectly by the compiler, and compile out if not */
static_assert(sizeof(nk_short) == 2);
static_assert(sizeof(nk_ushort) == 2);
static_assert(sizeof(nk_uint) == 4);
static_assert(sizeof(nk_int) == 4);
static_assert(sizeof(nk_byte) == 1);
static_assert(sizeof(nk_flags) >= 4);
static_assert(sizeof(nk_rune) >= 4);
static_assert(sizeof(nk_size) >= sizeof(void*));
static_assert(sizeof(nk_ptr) >= sizeof(void*));
static_assert(sizeof(nk_bool) == sizeof(bool));

#endif //NUKLEARCPP_NUKLEAR_BASIC_H
