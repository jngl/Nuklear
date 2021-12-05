//
// Created by jngl on 05/12/2021.
//

#ifndef NUKLEARCPP_NUKLEAR_DEP_H
#define NUKLEARCPP_NUKLEAR_DEP_H

void* nk_memcopy(void *dst, const void *src, unsigned int n);
void nk_memset(void *ptr, int c0, unsigned int size);
char *nk_dtoa(char *s, double n);

float nk_sin(float x);
float nk_cos(float x);

#define NK_MEMCPY nk_memcopy
#define NK_MEMSET nk_memset
#define NK_DTOA nk_dtoa

#define NK_SIN nk_sin
#define NK_COS nk_cos

#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

constexpr unsigned int NK_INPUT_MAX = 16;
constexpr unsigned int NK_MAX_NUMBER_BUFFER = 64;
constexpr float NK_SCROLLBAR_HIDING_TIMEOUT = 4.0f;

#endif //NUKLEARCPP_NUKLEAR_DEP_H
