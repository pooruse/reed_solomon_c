#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "reed_solomon.h"
#include "galois.h"

static uint8_t test_buffer[NELE];
void test(char *msg, int len, char *err, int len_err) {
    int ret;
    
    struct gf_poly msg_in;
    struct gf_poly err_in;
    
    memcpy(test_buffer, msg, len);
    msg_in.dat = (uint8_t *)test_buffer;
    msg_in.len = len;
    rs_encode_msg(&msg_in);

    err_in.dat = (uint8_t *)err;
    err_in.len = len_err;
    
    gf_poly_add(&msg_in, &msg_in, &err_in);

    ret = rs_decode_msg(&msg_in);
    if(ret == REED_SOLOMON_NO_ERROR) {
	printf("No error in the message. \n");
    } else if(ret == REED_SOLOMON_ERROR_CORRECT) {
	printf("Error in the message and is been corrected.\n");
    } else if(ret == REED_SOLOMON_TOO_MUCH_ERROR) {
	printf("Too much error.\n");
    } else {
	printf("Shouldn't be here.\n");
    }
    
}


int main(void)
{
    rs_init();
    test("01234567", 7, "\0\0\0\0\0\0", 6);
    return 0;
}
