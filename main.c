#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "reed_solomon.h"
#include "galois.h"

static struct gf_poly temp_poly;
static uint8_t temp_buffer[NELE];
static uint8_t msg_buffer[NELE];
static uint8_t expect[NELE];

void test(char *msg, int len, char *err, int len_err, int ret_exp) {
    int i;
    int ret;
    
    struct gf_poly msg_in;
    struct gf_poly err_in;

    memcpy(msg_buffer, msg, len);
    msg_in.dat = (uint8_t *)msg_buffer;
    msg_in.len = len;
    rs_encode_msg(&msg_in);

    memcpy(expect, msg_in.dat, msg_in.len);
    
    err_in.dat = (uint8_t *)err;
    err_in.len = len_err;

    gf_poly_add(&temp_poly, &msg_in, &err_in);
    gf_poly_copy(&msg_in, &temp_poly);
    
    ret = rs_decode_msg(&msg_in);
    if(ret == REED_SOLOMON_ERROR_CORRECT || ret == REED_SOLOMON_NO_ERROR) {
	for(i = 0; i < msg_in.len; i++) {
	    if(msg_in.dat[i] != expect[i]) {
		printf("i: %d, expect: %02X, result: %02X\n", i, expect[i], msg_in.dat[i]);
	    }
	}
    }
    if(ret == ret_exp) {
	printf("PASS\n");
	return;
    } else {
	printf("ret: %d, ret_exp: %d\n", ret, ret_exp);
    }
}


int main(void)
{
    // initial memory for test
    temp_poly.dat = temp_buffer;
    temp_poly.len = 0;

    // initial reed_solomon
    rs_init();

    // start test
    test("01234567", 8, "q\0\0\0\0c\0\0\0\0\0\0", 12, REED_SOLOMON_ERROR_CORRECT);
    test("01234567", 8, "q\0\0\0\0c\0\0\0\0\0\0", 12, REED_SOLOMON_ERROR_CORRECT);
    test("01234567", 8, "qb\0\0\0\0\0\0\0\0\0\0", 12, REED_SOLOMON_ERROR_CORRECT);
    test("basdfeqd", 8, "q\0\0\0\0\0\0\0\0\0\0\0", 12, REED_SOLOMON_ERROR_CORRECT);
    test("1234kcvj", 8, "q\0\0\0a\0\0\0\0\0\0\0", 12, REED_SOLOMON_ERROR_CORRECT);
    test("akjviu23", 8, "q\0\0\0\0\0\0e\0\0\0\0", 12, REED_SOLOMON_ERROR_CORRECT);
    test("ajk38723", 8, "q\0\0\0b\0\0\0\0\0\0\0", 12, REED_SOLOMON_ERROR_CORRECT);
    test("ajk38723", 8, "q\0\0\0b\0\0\0\0\0e\0", 12, REED_SOLOMON_TOO_MUCH_ERROR);
    test("ajk38723", 8, "q\0\0\0b\0a\0\0\0e\0", 12, REED_SOLOMON_TOO_MUCH_ERROR);
    test("ajk38723", 8, "\0\0\0\0\0\0\0\0\0\0\0\0", 12, REED_SOLOMON_NO_ERROR);
    return 0;
}
