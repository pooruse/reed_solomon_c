#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "reed_solomon.h"
#include "galois.h"

static struct gf_poly temp_poly;
static uint8_t temp_buffer[NELE];
static uint8_t msg_buffer[NELE];
static uint8_t expect[NELE];

void test(char *msg, int len, char *err, int len_err) {
    int i;
    int ret;
    
    struct gf_poly msg_in;
    struct gf_poly err_in;

    printf("msg: ");
    for(i = 0; i < len; i++) {
	printf("%02X ", msg[i]);
    }
    printf("\n");
    printf("err: ");
    for(i = 0; i < len; i++) {
	printf("%02X ", err[i]);
    }
    printf("\n");
    
    
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
    
    if(ret == REED_SOLOMON_NO_ERROR) {
	printf("No error in the message. \n");
    } else if(ret == REED_SOLOMON_ERROR_CORRECT) {
	printf("Error in the message.\n");
	ret = 0;
	for(i = 0; i < msg_in.len; i++) {
	    if(msg_in.dat[i] != expect[i]) {
		ret = 1;
		printf("i: %d, expect: %02X, result: %02X\n", i, expect[i], msg_in.dat[i]);
	    }
	}

	if(ret) {
	    printf("Error correct fail.\n");
	} else {
	    printf("Error correct success.\n");
	}
	
    } else if(ret == REED_SOLOMON_TOO_MUCH_ERROR) {
	printf("Too much error.\n");
    } else {
	printf("Shouldn't be here.\n");
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
    test("01234567", 8, "q\0\0\0\0c\0\0\0\0\0\0", 12);
    test("01234567", 8, "q\0\0\0\0c\0\0\0\0\0\0", 12);
    test("01234567", 8, "qb\0\0\0\0\0\0\0\0\0\0", 12);
    test("basdfeqd", 8, "q\0\0\0\0\0\0\0\0\0\0\0", 12);
    test("1234kcvj", 8, "q\0\0\0a\0\0\0\0\0\0\0", 12);
    test("akjviu23", 8, "q\0\0\0\0\0\0e\0\0\0\0", 12);
    test("ajk38723", 8, "q\0\0\0b\0\0\0\0\0\0\0", 12);
    test("ajk38723", 8, "q\0\0\0b\0\0\0\0\0e\0", 12);
    test("ajk38723", 8, "q\0\0\0b\0a\0\0\0e\0", 12);
    test("ajk38723", 8, "\0\0\0\0\0\0\0\0\0\0\0\0", 12);
    return 0;
}
