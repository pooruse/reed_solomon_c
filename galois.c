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

int gf_poly_scale(uint8_t *r, uint8_t  *p, int len, uint8_t x)
{
    int i;
    for(i = 0; i < len; i++) {
        r[i] = gf_mul(p[i], x);
    }
    
    return len;
}

int gf_poly_add(uint8_t *r, uint8_t  *p, int lenp, uint8_t *q, int lenq)
{
    int i;
    int lenr;
    
    if(lenp > lenq) {
        lenr = lenp;
    } else {
        lenr = lenq;
    }
    
    for(i = 0; i < lenp; i++) {
        r[i+lenr-lenp] = p[i]
    }

    for(i = 0; i < lenq; i++) {
        r[i+lenr-lenq] ^= q[i];
    }

    return lenr;
}

int gf_poly_mul(uint8_t *r, uint8_t *p, int lenp, uint8_t *q, int lenq)
{
    int i, j;
    for(j = 0; j < lenq; j++) {
        for(i = 0; i < lenp; i++) {
            r[i+j] ^= gf_mul(p[i], q[j]);
        }
    }

    return lenp + lenq - 1;
}

uint8_t gf_poly_eval(uint8_t *p, int lenp, uint8_t x)
{
    uint8_t y;
    y = p[0];
    for(i = 1; i < lenp; i++) {
        y = gf_mul(y, x) ^ p[i];
    }

    return y;
}

void gf_poly_div(uint8_t *r, int *lenq, int *lenr,
                    uint8_t *dividend, int len_dividend,
                    uint8_t *divisor, int len_divisor)
{
    int i, j;
    uint8_t normalizer;

    normalizer = divisor[0];
    gf_poly_scale(divisor, divisor, len_divisor, gf_inv(normalizer));

    // copy dividend to result
    for(i = 0; i < len_dividend; i++) {
        r[i] = dividend[i];
    }
    
    for(i = 0; i < (len_dividend - len_divisor + 1) ; i++) {
        if(r[i] == 0) {
            continue;
        }
        
        for(j = 1; j < len_divisor; j++) {
            if(divisor[j] == 0) {
                continue;
            }

            r[i+j] ^= gf_mul(r[i], divisor[j]);
        }
    }

    *lenq = len_dividend - len_divisor + 1;
    *lenr = len_divisor - 1;
    gf_poly_scale(r, r, len_dividend - len_divisor, gf_inv(normalizer));
    gf_poly_scale(divisor, divisor, len_divisor, gf_inv(normalizer));
}

int gf_poly_len(uint8_t *p, int lenp)
{
    int i;
    int ret = 0;
    for(i = 0; i < lenp; i++) {
        if(p[i] != 0) {
            ret = i+1;
        }
    }

    return ret;
}
   


