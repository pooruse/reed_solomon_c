#include <string.h>
#include "galois.h"

const uint8_t gf_log[] =
{
    0, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75, 4, 100, 224, 14, 52, 141, 239, 129, 28, 193, 105, 248, 200, 8, 76, 113, 5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240, 18, 130, 69, 29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166, 6, 191, 139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136, 54, 208, 148, 206, 143, 150, 219, 189, 241, 210, 19, 92, 131, 56, 70, 64, 30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250, 133, 186, 61, 202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87, 7, 112, 192, 247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24, 227, 165, 153, 119, 38, 184, 180, 124, 17, 68, 146, 217, 35, 32, 137, 46, 55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97, 242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162, 31, 45, 67, 216, 183, 123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246, 108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134, 177, 187, 204, 62, 90, 203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215, 79, 174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175
};

const uint8_t gf_exp[] =
{
    1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 0, 0
};

uint8_t gf_div(uint8_t x, uint8_t y)
{
    if(x == 0) {
        return 0;
    }

    if(y == 0) {
        // Divide zero error
        return 0;
    }
    
    return gf_exp[((gf_log[x] + NELE - 1) - gf_log[y])];
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
void gf_poly_scale(struct gf_poly *p, uint8_t x)
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
void gf_poly_add(struct gf_poly *r, struct gf_poly *p, struct gf_poly *q)
{
    int i;
    int lenr;
    
    if(p->len > q->len) {
        lenr = p->len;
    } else {
        lenr = q->len;
    }
    
    for(i = 0; i < p->len; i++) {
        r->dat[i + lenr - p->len] = p->dat[i];
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
void gf_poly_mul(struct gf_poly *r, struct gf_poly *p, struct gf_poly *q)
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
    int i;
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

    normalizer = divisor->dat[0];
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
            return;
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
