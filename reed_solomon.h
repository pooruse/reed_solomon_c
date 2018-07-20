#ifndef REED_SOLOMON_H
#define REED_SOLOMON_H

#include <stdint.h>
#include "galois.h"

#define NSYM 4

#define REED_SOLOMON_NO_ERROR 0
#define REED_SOLOMON_ERROR_CORRECT 1
#define REED_SOLOMON_TOO_MUCH_ERROR -1

void rs_init(void);
void rs_generator_poly(struct gf_poly *gen, int nsym);
void rs_encode_msg(struct gf_poly *msg);
void rs_calc_syndromes(struct gf_poly *msg, struct gf_poly *synd);
int rs_check_syndromes(struct gf_poly *synd);

void rs_find_error_locator_and_evaluator(struct gf_poly *locator,
                                         struct gf_poly *evaluator,
                                         struct gf_poly *synd);

void rs_correct_errata(struct gf_poly *msg, uint8_t *err_loc, int len_err_loc,
                       struct gf_poly *locator, struct gf_poly *evaluator);

void rs_find_errors(uint8_t *err_loc, int *len_err_loc, struct gf_poly *locator, int nmsg);
int rs_decode_msg(struct gf_poly *msg);
#endif
