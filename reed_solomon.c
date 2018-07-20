#include "reed_solomon.h"


static struct gf_poly code_generator;
static uint8_t code_generator_buffer = [NSYM + 1];
static struct gf_poly temp_poly;
static uint8_t temp_buffer[NELE];

void rs_init(void)
{
    code_generator.dat = code_generator_buffer;
    rs_generator_poly(&code_generator, NSYM);
    temp_poly.dat = temp_buffer;
}

void rs_generator_poly(struct gf_poly *gen, int nsym)
{
    int i;
    uint8_t base_buf[2];
    static gf_poly base;
    
    base.len = 2;
    base.dat = base_buf;
    
    base[0] = 1;
    gen->dat[0] = 0;
    gen->len = 1;
    for(i = 0; i < nsym; i++) {
        base[1] = gf_pow(2,i);
        gf_poly_mul(&temp_poly, gen, base);
        gf_poly_copy(gen, &temp_poly);
    }
}

void rs_encode_msg(struct gf_poly *msg)
{
    struct gf_poly remainder;
    gf_poly_div(&temp_poly, &remainder, msg, &code_generator);
    gf_poly_append(msg, &remainder);
}

void rs_calc_syndromes(struct gf_poly *msg, struct gf_poly *synd)
{
    int i;
    for(i = 0; i < NSYM; i++) {
        synd->dat[i] = gf_poly_eval(msg, gf_exp[i]);
    }
    synd->len = NSYM;
}

int rs_check_syndromes(struct gf_poly *synd)
{
    int i;
    for(i = 0; i < NSYM; i++) {
        if(synd->dat[i] != 0) {
            return 1;
        }
    }

    return 0;
}

void rs_find_error_locator_and_evaluator(struct gf_poly *locator,
                                         struct gf_poly *evaluator,
                                         struct gf_poly *synd)
{
    int i;

    struct gf_poly remainder;
    struct gf_poly v_pre; uint8_t v_pre_buf[NSYM + 1];
    struct gf_poly v_cur; uint8_t v_cur_buf[NSYM + 1];
    struct gf_poly x_pre; uint8_t x_pre_buf[NSYM - 1];
    struct gf_poly x_cur; uint8_t x_cur_buf[NSYM - 1];
    struct gf_poly x_tmp; uint8_t x_tmp_buf[NSYM - 1];
    struct gf_poly x_tmp2;
        
    v_pre.dat = v_pre_buf; v_pre.len = NSYM + 1;
    v_cur.dat = v_cur_buf; v_cur.len = NSYM + 1;
    x_pre.dat = x_pre_buf; x_pre.len = 1;
    x_cur.dat = x_cur_buf; x_cur.len = 1;
    x_tmp.dat = x_tmp_buf; x_cur.len = 0;

    for(i = 0; i < v_pre.len; i++) {
        v_pre.dat[i] = 0;
    }
    v_pre.dat[0] = 1;

    gf_poly_copy(&v_cur, synd);
    x_pre.dat[0] = 0;
    x_cur.dat[0] = 1;

    while(1) {
        if(v_cur.len <= (NSYM/2)) {
            break;
        }

        gf_poly_div(&temp_poly, &remainder, &v_pre, &v_cur);
        gf_poly_copy(&v_pre, &v_cur);
        gf_poly_copy(&v_cur, &remainder);
        gf_poly_copy(&x_tmp, &x_pre);
        gf_poly_copy(&x_pre, &x_cur);

        // save the internal caculation data after remainder
        // the thing save in temp_buffer will become
        // | quotient(? bytes)  |  remainder (bytes) | x_tmps (bytes) |
        x_tmp2.dat = &(remainder.dat[remainder.len]);
        gf_poly_mul(&x_tmp2, &temp_poly, &x_cur);
        gf_poly_add(&x_cur, &x_tmp, &x_tmp2);
        gf_poly_delete_leading_zeros(&v_cur);
    }

    gf_poly_copy(locator, &x_cur);
    gf_poly_copy(evaluator, &v_cur);
}

void rs_correct_errata(uint8_t *msg, uint8_t *err_loc, int len_err_loc,
                       struct gf_poly *locator, struct gf_poly *evaluator)
{
    int i;
    uint8_t x, x_inv;
    uint8_t dividend, divisor;
    uint8_t mag;
    int msg_i;
    
    // deriviation
    for(i = 0; i < locator->len; i++) {
        locator[locator->len - i - 1] = 0;
    }

    for(i = 0; i < len_err_loc; i++) {
        x = gf_exp[err_loc[i]];
        x_inv = gf_inv(x);
        dividend = gf_poly_eval(evaluator, x_inv);
        divisor = gf_poly_eval(locator, x_inv);
        mag = gf_mul(x, gf_div(dividend, divisor));
        msg_i = msg->len - err_loc[i] - 1;
        msg->dat[msg_i] = mag ^ msg->dat[msg_i];
    }
}

void rs_find_errors(uint8_t *err_loc, int *len_err_loc, struct gf_poly *locator, int nmsg)
{
    int i, j;
    uint8_t a;
    uint8_t tmp;
    int i_err;
    int i_locator;
    
    a = gf_inv(gf_exp[NELE - nmsg]);
    for(i = 0; i < locator->len; i++) {
        gf_poly_copy(&temp_poly, gf_pow(a, i));
    }

    i_err = 0;
    for(i = 0; i < nmsg; i++) {
        tmp = 0;
        for(j = 0; j < temp_poly.len; j++) {
            tmp ^= temp_poly.dat[j];
        }

        if(tmp == 0) {
            i_err++;
            if(i_err > (NSYM/2)) {
                // too many errors
                return;
            }
            err_loc[i_err] = nmsg - i - 1;
        }

        for(j = 0; j < temp_poly.len; j++) {
            i_locator = locator->len - j - 1;
            temp_poly.dat[i_locator] = gf_mul(temp_poly.dat[i_locator], gf_exp[j])
        }
    }
}

int rs_decode_msg(struct gf_poly *msg)
{
    struct gf_poly syndromes;
    uint8_t syndromes_buf[NSYM + 1];
    
    struct gf_poly locator;
    uint8_t locator_buf[NSYM + 1];
    
    struct gf_poly evaluator;
    uint8_t evaluator_buf[NSYM/2];
    
    uint8_t err_loc[NSYM/2];
    int len_err_loc;
    
    rs_calc_syndromes(msg, syndromes);
    if(!rs_check_syndromes(&syndromes)) {
        return REED_SOLOMON_NO_ERROR;
    }

    rs_find_error_locator_and_evaluator(&locator,
                                        &evaluator,
                                        &syndromes);
    rs_find_errors(err_loc, &len_err_loc, locator, msg->len);
    if(len_err_loc > (NSYM/2)) {
        return REED_SOLOMON_TOO_MUCH_ERROR;
    }

    rs_correct_errata(msg, err_loc, len_err_loc,
                      locator,
                      evaluator);

    return REED_SOLOMON_ERROR_CORRECT;
}
