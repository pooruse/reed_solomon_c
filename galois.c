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

viod gf_poly_scale(struct gf_poly *r, struct gf_poly *p, uint8_t x)
{
    int i;
    for(i = 0; i < p->len; i++) {
        r->dat[i] = gf_mul(p->dat[i], x);
    }
}

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

uint8_t gf_poly_eval(struct gf_poly *p, uint8_t x)
{
    uint8_t y;
    y = p->dat[0];
    for(i = 1; i < p->len; i++) {
        y = gf_mul(y, x) ^ p->dat[i];
    }

    return y;
}

void gf_poly_div(struct gf_poly *q, struct gf_poly *r,
                 struct gf_poly *dividend, struct gf_poly *divisor);
{
    int i, j;
    uint8_t normalizer;

    normalizer = divisor[0];
    gf_poly_scale(divisor, divisor, gf_inv(normalizer));

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
    r->len = divisor->len - 1;

    for(i = 0; i < r->len; i++) {
        r->dat[i] = q->dat[q->len + i];
    }
    
    gf_poly_scale(q, q, gf_inv(normalizer));
    gf_poly_scale(divisor, divisor, gf_inv(normalizer));
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
   


