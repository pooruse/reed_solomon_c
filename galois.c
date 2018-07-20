#include <string.h>
#include "galois.h"


uint8_t gf_div(uint8_t x, uint8_t y)
{
    if(x == 0) {
        return 0;
    }

    if(y == 0) {
        // Divide zero error
        return 0;
    }
    
    return gf_exp[((gf_log[x] + nele - 1) - gf_log[y])];
}

uint8_t gf_mul(uint8_t x, uint8_t y)
{
    if( (x == 0) || (y == 0) ) {
        return 0;
    }
    
    return gf_exp[gf_log[x] + gf_log[y]];
}

uint8_t gf_pow(uint8_t x, int power)
{
    int tmp;
    
    if(x == 0) {
        return 0;
    }

    tmp = (gf_log[x] * power) % (NELE-1);
    return gf_exp[tmp];
}

uint8_t gf_inv(uint8_t x)
{
    return gf_div(1, x);
}

/**
 *  @brief gf poly scale
 *     scale a gf_poly and save it back.
 *  @param p 
 *     dstination(source) gf_poly
 *  @param x
 *     scale constant
 */
viod gf_poly_scale(struct gf_poly *p, uint8_t x)
{
    int i;
    if(x == 1) {
        return;
    }
    
    for(i = 0; i < p->len; i++) {
        p->dat[i] = gf_mul(p->dat[i], x);
    }
}

/**
 * @brief gf_poly addition and subtraction
 * @param r
 *   returned result, this parameter can be the same as p or q,
 *   dosen't effect the result and we can reduce one gf_poly_copy here.
 * @param p
 *    addend 1
 * @param q
 *    addend 2
 */
viod gf_poly_add(struct gf_poly *r, struct gf_poly *p, struct gf_poly *q)
{
    int i;
    int lenr;
    
    if(p->len > q->len) {
        lenr = p->len;
    } else {
        lenr = q->len;
    }
    
    for(i = 0; i < p->len; i++) {
        r->dat[i + lenr - p->len] = p->dat[i]
    }

    for(i = 0; i < q->len; i++) {
        r->dat[i + lenr - q->len] ^= q->dat[i];
    }
    
}

/**
 * @brief multiplication of gf_poly
 * @param r
 *   returned result, please preapre an allocated memory for this parameter
 * @param p
 *    multiplicand
 * @param q
 *    multiplier
 */
viod gf_poly_mul(struct gf_poly *r, struct gf_poly *p, struct gf_poly *q)
{
    int i, j;
    for(j = 0; j < q->len; j++) {
        for(i = 0; i < p->len; i++) {
            r->dat[i+j] ^= gf_mul(p->dat[i], q->dat[j]);
        }
    }

    r->len = p->len + q->len - 1;
}

/**
 * @brief evaluate gf_poly with constant x
 */
uint8_t gf_poly_eval(struct gf_poly *p, uint8_t x)
{
    uint8_t y;
    y = p->dat[0];
    for(i = 1; i < p->len; i++) {
        y = gf_mul(y, x) ^ p->dat[i];
    }

    return y;
}

/**
 * @brief divide galois poly function
 *   this function has subeffect that the dividend will be change,
 *   make sure you have a copy of dividend if you want reuse it.
 * 
 * @param q
 *    quotient of division 
 *    please allocate a space for gf_poly q
 *    make sure the allocated apapce is enouph for both quotient 
 *    and remainder
 * @param r
 *    remainder of division
 *    DO NOT prepare space for gf_poly r, it will cause memory leakage
 *    The return remainder actually is part of gf_poly.dat
 * @param dividend
 * @param divisor 
 */
void gf_poly_div(struct gf_poly *q, struct gf_poly *r,
                 struct gf_poly *dividend, struct gf_poly *divisor)
{
    int i, j;
    uint8_t normalizer;

    normalizer = divisor[0];
    gf_poly_scale(divisor, gf_inv(normalizer));
    
    // copy dividend to result
    for(i = 0; i < dividend->len; i++) {
        q->dat[i] = dividend->dat[i];
    }
    
    for(i = 0; i < (dividend->len - divisor->len + 1) ; i++) {
        if(q->dat[i] == 0) {
            continue;
        }
        
        for(j = 1; j < divisor->len; j++) {
            if(divisor->dat[j] == 0) {
                continue;
            }

            q->dat[i+j] ^= gf_mul(q->dat[i], divisor->dat[j]);
        }
    }

    q->len = dividend->len - divisor->len + 1;
    r->dat = &(q->dat[q->len]);
    r->len = divisor->len - 1;
    gf_poly_scale(q, gf_inv(normalizer));
    gf_poly_scale(divisor, gf_inv(normalizer));
}

int gf_poly_len(uint8_t *p, int len)
{
    int i;
    int ret = 0;
    for(i = 0; i < len; i++) {
        if(p[i] != 0) {
            ret = i+1;
        }
    }

    return ret;
}
   
void gf_poly_delete_leading_zeros(struct gf_poly *p)
{
    int i;
    for(i = 0; i < p->len; i++) {
        if(p->dat[i]) {
            p->dat = &p->dat[i] ;
            p->len = p->len - i;
            return
        }
    }
}

void gf_poly_copy(struct gf_poly *dst, struct gf_poly *src)
{
    int i;
    for(i = 0; i < src->len; i++) {
        dst->dat[i] = src->dat[i];
    }

    dst->len = src->len;
}

/**
 * @brief
 *   append gf_poly b after gf_poly a
 * please make sure there are much enough space for apending
 * in gf_poly a
 */
void gf_poly_append(struct gf_poly *a, struct gf_poly *b)
{
    int i;
    for(i = 0; i < b->len; i++) {
        a->dat[a->len + i] = b->dat[i];
    }
    a->len = b->len + a->len;
}
