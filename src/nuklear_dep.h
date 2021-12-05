//
// Created by jngl on 05/12/2021.
//

#ifndef NUKLEARCPP_NUKLEAR_DEP_H
#define NUKLEARCPP_NUKLEAR_DEP_H

NK_LIB void* nk_memcopy(void *dst, const void *src, nk_size n);
NK_LIB void nk_memset(void *ptr, int c0, nk_size size);
NK_LIB char *nk_dtoa(char *s, double n);

NK_LIB float nk_sin(float x);
NK_LIB float nk_cos(float x);

#define NK_MEMCPY nk_memcopy
#define NK_MEMSET nk_memset
#define NK_DTOA nk_dtoa

#define NK_SIN nk_sin
#define NK_COS nk_cos

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#endif //NUKLEARCPP_NUKLEAR_DEP_H
