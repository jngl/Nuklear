#include "nuklear.h"
#include "nuklear_internal.h"

/* ===============================================================
 *
 *                              MATH
 *
 * ===============================================================*/
/*  Since nuklear is supposed to work on all systems providing floating point
    math without any dependencies I also had to implement my own math functions
    for sqrt, sin and cos. Since the actual highly accurate implementations for
    the standard library functions are quite complex and I do not need high
    precision for my use cases I use approximations.

    Sqrt
    ----
    For square root nuklear uses the famous fast inverse square root:
    https://en.wikipedia.org/wiki/Fast_inverse_square_root with
    slightly tweaked magic constant. While on today's hardware it is
    probably not faster it is still fast and accurate enough for
    nuklear's use cases. IMPORTANT: this requires float format IEEE 754

    Sine/Cosine
    -----------
    All constants inside both function are generated Remez's minimax
    approximations for value range 0...2*PI. The reason why I decided to
    approximate exactly that range is that nuklear only needs sine and
    cosine to generate circles which only requires that exact range.
    In addition I used Remez instead of Taylor for additional precision:
    www.lolengine.net/blog/2011/12/21/better-function-approximations.

    The tool I used to generate constants for both sine and cosine
    (it can actually approximate a lot more functions) can be
    found here: www.lolengine.net/wiki/oss/lolremez
*/
float
nk_inv_sqrt(float n)
{
    float x2;
    const float threehalfs = 1.5f;
    union {nk_uint i; float f;} conv = {0};
    conv.f = n;
    x2 = n * 0.5f;
    conv.i = 0x5f375A84 - (conv.i >> 1);
    conv.f = conv.f * (threehalfs - (x2 * conv.f * conv.f));
    return conv.f;
}
nk_uint
nk_round_up_pow2(nk_uint v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}
double
nk_pow(double x, int n)
{
    /*  check the sign of n */
    double r = 1;
    int plus = n >= 0;
    n = (plus) ? n : -n;
    while (n > 0) {
        if ((n & 1) == 1)
            r *= x;
        n /= 2;
        x *= x;
    }
    return plus ? r : 1.0 / r;
}
int
nk_ifloord(double x)
{
    x = (double)((int)x - ((x < 0.0) ? 1 : 0));
    return (int)x;
}
int
nk_ifloorf(float x)
{
    x = (float)((int)x - ((x < 0.0f) ? 1 : 0));
    return (int)x;
}
int
nk_iceilf(float x)
{
    if (x >= 0) {
        int i = (int)x;
        return (x > i) ? i+1: i;
    } else {
        int t = (int)x;
        float r = x - (float)t;
        return (r > 0.0f) ? t+1: t;
    }
}
int
nk_log10(double n)
{
    int neg;
    int ret;
    int exp = 0;

    neg = (n < 0) ? 1 : 0;
    ret = (neg) ? (int)-n : (int)n;
    while ((ret / 10) > 0) {
        ret /= 10;
        exp++;
    }
    if (neg) exp = -exp;
    return exp;
}
 struct nk_rect
nk_get_null_rect(void)
{
    return nk_null_rect;
}
 struct nk_rect
nk_rect(float x, float y, float w, float h)
{
    struct nk_rect r;
    r.x = x; r.y = y;
    r.w = w; r.h = h;
    return r;
}
 struct nk_rect
nk_recti(int x, int y, int w, int h)
{
    struct nk_rect r;
    r.x = (float)x;
    r.y = (float)y;
    r.w = (float)w;
    r.h = (float)h;
    return r;
}
 struct nk_rect
nk_recta(struct nk_vec2 pos, struct nk_vec2 size)
{
    return nk_rect(pos.x, pos.y, size.x, size.y);
}
 struct nk_rect
nk_rectv(const float *r)
{
    return nk_rect(r[0], r[1], r[2], r[3]);
}
 struct nk_rect
nk_rectiv(const int *r)
{
    return nk_recti(r[0], r[1], r[2], r[3]);
}
 struct nk_vec2
nk_rect_pos(struct nk_rect r)
{
    struct nk_vec2 ret;
    ret.x = r.x; ret.y = r.y;
    return ret;
}
 struct nk_vec2
nk_rect_size(struct nk_rect r)
{
    struct nk_vec2 ret;
    ret.x = r.w; ret.y = r.h;
    return ret;
}
struct nk_rect
nk_shrink_rect(struct nk_rect r, float amount)
{
    struct nk_rect res;
    r.w = NK_MAX(r.w, 2 * amount);
    r.h = NK_MAX(r.h, 2 * amount);
    res.x = r.x + amount;
    res.y = r.y + amount;
    res.w = r.w - 2 * amount;
    res.h = r.h - 2 * amount;
    return res;
}
struct nk_rect
nk_pad_rect(struct nk_rect r, struct nk_vec2 pad)
{
    r.w = NK_MAX(r.w, 2 * pad.x);
    r.h = NK_MAX(r.h, 2 * pad.y);
    r.x += pad.x; r.y += pad.y;
    r.w -= 2 * pad.x;
    r.h -= 2 * pad.y;
    return r;
}
 struct nk_vec2
nk_vec2(float x, float y)
{
    struct nk_vec2 ret;
    ret.x = x; ret.y = y;
    return ret;
}
 struct nk_vec2
nk_vec2i(int x, int y)
{
    struct nk_vec2 ret;
    ret.x = (float)x;
    ret.y = (float)y;
    return ret;
}
 struct nk_vec2
nk_vec2v(const float *v)
{
    return nk_vec2(v[0], v[1]);
}
 struct nk_vec2
nk_vec2iv(const int *v)
{
    return nk_vec2i(v[0], v[1]);
}
void
nk_unify(struct nk_rect *clip, const struct nk_rect *a, float x0, float y0,
    float x1, float y1)
{
    NK_ASSERT(a);
    NK_ASSERT(clip);
    clip->x = NK_MAX(a->x, x0);
    clip->y = NK_MAX(a->y, y0);
    clip->w = NK_MIN(a->x + a->w, x1) - clip->x;
    clip->h = NK_MIN(a->y + a->h, y1) - clip->y;
    clip->w = NK_MAX(0, clip->w);
    clip->h = NK_MAX(0, clip->h);
}

 void
nk_triangle_from_direction(struct nk_vec2 *result, struct nk_rect r,
    float pad_x, float pad_y, enum nk_heading direction)
{
    float w_half, h_half;
    NK_ASSERT(result);

    r.w = NK_MAX(2 * pad_x, r.w);
    r.h = NK_MAX(2 * pad_y, r.h);
    r.w = r.w - 2 * pad_x;
    r.h = r.h - 2 * pad_y;

    r.x = r.x + pad_x;
    r.y = r.y + pad_y;

    w_half = r.w / 2.0f;
    h_half = r.h / 2.0f;

    if (direction == NK_UP) {
        result[0] = nk_vec2(r.x + w_half, r.y);
        result[1] = nk_vec2(r.x + r.w, r.y + r.h);
        result[2] = nk_vec2(r.x, r.y + r.h);
    } else if (direction == NK_RIGHT) {
        result[0] = nk_vec2(r.x, r.y);
        result[1] = nk_vec2(r.x + r.w, r.y + h_half);
        result[2] = nk_vec2(r.x, r.y + r.h);
    } else if (direction == NK_DOWN) {
        result[0] = nk_vec2(r.x, r.y);
        result[1] = nk_vec2(r.x + r.w, r.y);
        result[2] = nk_vec2(r.x + w_half, r.y + r.h);
    } else {
        result[0] = nk_vec2(r.x, r.y + h_half);
        result[1] = nk_vec2(r.x + r.w, r.y);
        result[2] = nk_vec2(r.x + r.w, r.y + r.h);
    }
}

