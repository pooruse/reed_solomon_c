#ifndef GALOIS_H
#define GALOIS_H

#define NELE 256

#include <stdint.h>

extern const uint8_t gf_log[];
extern const uint8_t gf_exp[];

struct gf_poly {
    uint8_t *dat;
    int len;
};

uint8_t gf_div(uint8_t x, uint8_t y);
uint8_t gf_mul(uint8_t x, uint8_t y);
uint8_t gf_pow(uint8_t x, int power);
uint8_t gf_inv(uint8_t x);

void gf_poly_scale(struct gf_poly *p, uint8_t x);
void gf_poly_add(struct gf_poly *r, struct gf_poly *p, struct gf_poly *q);
void gf_poly_mul(struct gf_poly *r, struct gf_poly *p, struct gf_poly *q);
uint8_t gf_poly_eval(struct gf_poly *p, uint8_t x);

void gf_poly_div(struct gf_poly *q, struct gf_poly *r,
                 struct gf_poly *dividend, struct gf_poly *divisor);

int gf_poly_len(uint8_t *p, int len);
void gf_poly_delete_leading_zeros(struct gf_poly *p);
void gf_poly_copy(struct gf_poly *dst, struct gf_poly *src);
void gf_poly_append(struct gf_poly *a, struct gf_poly *b);

#endif
