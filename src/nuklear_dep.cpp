//
// Created by jngl on 05/12/2021.
//

#include "nuklear.h"
#include "nuklear_internal.h"

void* nk_memcopy(void *dst0, const void *src0, unsigned int length)
{
    nk_ptr t;
    char *dst = (char*)dst0;
    const char *src = (const char*)src0;
    if (length == 0 || dst == src)
        goto done;

#define nk_word int
#define nk_wsize sizeof(nk_word)
#define nk_wmask (nk_wsize-1)
#define NK_TLOOP(s) if (t) NK_TLOOP1(s)
#define NK_TLOOP1(s) do { s; } while (--t)

    if (dst < src) {
        t = (nk_ptr)src; /* only need low bits */
        if ((t | (nk_ptr)dst) & nk_wmask) {
            if ((t ^ (nk_ptr)dst) & nk_wmask || length < nk_wsize)
                t = length;
            else
                t = nk_wsize - (t & nk_wmask);
            length -= t;
            NK_TLOOP1(*dst++ = *src++);
        }
        t = length / nk_wsize;
        NK_TLOOP(*(nk_word*)(void*)dst = *(const nk_word*)(const void*)src;
                         src += nk_wsize; dst += nk_wsize);
        t = length & nk_wmask;
        NK_TLOOP(*dst++ = *src++);
    } else {
        src += length;
        dst += length;
        t = (nk_ptr)src;
        if ((t | (nk_ptr)dst) & nk_wmask) {
            if ((t ^ (nk_ptr)dst) & nk_wmask || length <= nk_wsize)
                t = length;
            else
                t &= nk_wmask;
            length -= t;
            NK_TLOOP1(*--dst = *--src);
        }
        t = length / nk_wsize;
        NK_TLOOP(src -= nk_wsize; dst -= nk_wsize;
                         *(nk_word*)(void*)dst = *(const nk_word*)(const void*)src);
        t = length & nk_wmask;
        NK_TLOOP(*--dst = *--src);
    }
#undef nk_word
#undef nk_wsize
#undef nk_wmask
#undef NK_TLOOP
#undef NK_TLOOP1
    done:
    return (dst0);
}

void nk_memset(void *ptr, int c0, unsigned int size)
{
    #define nk_word unsigned
    #define nk_wsize sizeof(nk_word)
    #define nk_wmask (nk_wsize - 1)
    nk_byte *dst = (nk_byte*)ptr;
    unsigned c = 0;
    nk_size t = 0;

    if ((c = (nk_byte)c0) != 0) {
        c = (c << 8) | c; /* at least 16-bits  */
        if (sizeof(unsigned int) > 2)
            c = (c << 16) | c; /* at least 32-bits*/
    }

    /* too small of a word count */
    dst = (nk_byte*)ptr;
    if (size < 3 * nk_wsize) {
        while (size--) *dst++ = (nk_byte)c0;
        return;
    }

    /* align destination */
    if ((t = NK_PTR_TO_UINT(dst) & nk_wmask) != 0) {
        t = nk_wsize -t;
        size -= t;
        do {
            *dst++ = (nk_byte)c0;
        } while (--t != 0);
    }

    /* fill word */
    t = size / nk_wsize;
    do {
        *(nk_word*)((void*)dst) = c;
        dst += nk_wsize;
    } while (--t != 0);

    /* fill trailing bytes */
    t = (size & nk_wmask);
    if (t != 0) {
        do {
            *dst++ = (nk_byte)c0;
        } while (--t != 0);
    }

    #undef nk_word
    #undef nk_wsize
    #undef nk_wmask
}

char* nk_dtoa(char *s, double n)
{
    int useExp = 0;
    int digit = 0, m = 0, m1 = 0;
    char *c = s;
    int neg = 0;

    NK_ASSERT(s);
    if (!s) return 0;

    if (n == 0.0) {
        s[0] = '0'; s[1] = '\0';
        return s;
    }

    neg = (n < 0);
    if (neg) n = -n;

    /* calculate magnitude */
    m = nk_log10(n);
    useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
    if (neg) *(c++) = '-';

    /* set up for scientific notation */
    if (useExp) {
        if (m < 0)
           m -= 1;
        n = n / (double)nk_pow(10.0, m);
        m1 = m;
        m = 0;
    }
    if (m < 1.0) {
        m = 0;
    }

    /* convert the number */
    while (n > NK_FLOAT_PRECISION || m >= 0) {
        double weight = nk_pow(10.0, m);
        if (weight > 0) {
            double t = (double)n / weight;
            digit = nk_ifloord(t);
            n -= ((double)digit * weight);
            *(c++) = (char)('0' + (char)digit);
        }
        if (m == 0 && n > 0)
            *(c++) = '.';
        m--;
    }

    if (useExp) {
        /* convert the exponent */
        int i, j;
        *(c++) = 'e';
        if (m1 > 0) {
            *(c++) = '+';
        } else {
            *(c++) = '-';
            m1 = -m1;
        }
        m = 0;
        while (m1 > 0) {
            *(c++) = (char)('0' + (char)(m1 % 10));
            m1 /= 10;
            m++;
        }
        c -= m;
        for (i = 0, j = m-1; i<j; i++, j--) {
            /* swap without temporary */
            c[i] ^= c[j];
            c[j] ^= c[i];
            c[i] ^= c[j];
        }
        c += m;
    }
    *(c) = '\0';
    return s;
}

float nk_sin(float x)
{
    NK_STORAGE const float a0 = +1.91059300966915117e-31f;
    NK_STORAGE const float a1 = +1.00086760103908896f;
    NK_STORAGE const float a2 = -1.21276126894734565e-2f;
    NK_STORAGE const float a3 = -1.38078780785773762e-1f;
    NK_STORAGE const float a4 = -2.67353392911981221e-2f;
    NK_STORAGE const float a5 = +2.08026600266304389e-2f;
    NK_STORAGE const float a6 = -3.03996055049204407e-3f;
    NK_STORAGE const float a7 = +1.38235642404333740e-4f;
    return a0 + x*(a1 + x*(a2 + x*(a3 + x*(a4 + x*(a5 + x*(a6 + x*a7))))));
}

float nk_cos(float x)
{
    /* New implementation. Also generated using lolremez. */
    /* Old version significantly deviated from expected results. */
    NK_STORAGE const float a0 = 9.9995999154986614e-1f;
    NK_STORAGE const float a1 = 1.2548995793001028e-3f;
    NK_STORAGE const float a2 = -5.0648546280678015e-1f;
    NK_STORAGE const float a3 = 1.2942246466519995e-2f;
    NK_STORAGE const float a4 = 2.8668384702547972e-2f;
    NK_STORAGE const float a5 = 7.3726485210586547e-3f;
    NK_STORAGE const float a6 = -3.8510875386947414e-3f;
    NK_STORAGE const float a7 = 4.7196604604366623e-4f;
    NK_STORAGE const float a8 = -1.8776444013090451e-5f;
    return a0 + x*(a1 + x*(a2 + x*(a3 + x*(a4 + x*(a5 + x*(a6 + x*(a7 + x*a8)))))));
}