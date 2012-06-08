/*
* krypt-core API - C version
*
* Copyright (C) 2011
* Hiroshi Nakamura <nahi@ruby-lang.org>
* Martin Bosslet <martin.bosslet@googlemail.com>
* All rights reserved.
*
* This software is distributed under the same license as Ruby.
* See the file 'LICENSE' for further details.
*/

#include "krypt-core.h"

VALUE mKryptHex;
VALUE cKryptHexEncoder;
VALUE cKryptHexDecoder;
VALUE eKryptHexError;

static const char krypt_hex_table[] = "0123456789abcdef";
static const char krypt_hex_table_inv[] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,-1,-1,
    -1,-1,-1,-1,-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,11,12,
    13,14,15
};

#define KRYPT_HEX_INV_MAX 102
#define KRYPT_HEX_DECODE 0
#define KRYPT_HEX_ENCODE 1

static int
int_hex_encode(unsigned char *bytes, size_t len, unsigned char *out)
{
    size_t i;
    unsigned char b;
    size_t j;
   
    for (i=0; i < len; i++) {
	b = bytes[i];
	j = i * 2;
	out[j] = krypt_hex_table[b >> 4];
	out[j + 1] = krypt_hex_table[b & 0x0f];
    }
    return 1;
}

static int
int_hex_decode(unsigned char *bytes, size_t len, unsigned char *out)
{
    size_t i;
    char b;
    unsigned char c, d;

    for (i=0; i < len / 2; i++) {
	c = (unsigned char) bytes[i*2];
	d = (unsigned char) bytes[i*2+1];
	if (c > KRYPT_HEX_INV_MAX || d > KRYPT_HEX_INV_MAX) {
	    krypt_error_add("Illegal hex character detected: %x or %x", c, d);
	    return 0;
	}
	b = krypt_hex_table_inv[c];
	if (b < 0) {
	    krypt_error_add("Illegal hex character detected: %x", c);
	    return 0;
	}
	out[i] = b << 4;
	b = krypt_hex_table_inv[d];
	if (b < 0) {
	    krypt_error_add("Illegal hex character detected: %x", d);
	    return 0;
	}
	out[i] |= b;
    }
    return 1;
}

#define int_hex_encode_tests(bytes, len, tmp)				\
do {									\
    if (!(bytes)) {							\
	(tmp) = -1;							\
    }									\
    if ((len) > SSIZE_MAX / 2) {					\
	krypt_error_add("Buffer too large: %ld", (len));		\
	(tmp) = -1;							\
    }									\
} while (0)

ssize_t
krypt_hex_encode(unsigned char *bytes, size_t len, unsigned char **out)
{
    ssize_t ret;
    unsigned char *retval;
    int tmp = 0;

    int_hex_encode_tests(bytes, len, tmp);
    if (tmp == -1)
	return -1;

    ret = 2 * len;
    retval = ALLOC_N(unsigned char, ret);
    int_hex_encode(bytes, len, retval);
    *out = retval;
    return ret;
}

#define int_hex_decode_tests(bytes, len, tmp)				\
do {									\
    if (!(bytes)) {							\
	(tmp) = -1;							\
    }									\
    if ((len) % 2) {							\
	krypt_error_add("Buffer length must be a multiple of 2");	\
	(tmp) = -1;							\
    }									\
    if ((len) / 2 > SSIZE_MAX) {					\
	krypt_error_add("Buffer too large: %ld", (len));		\
	(tmp) = -1;							\
    }									\
} while (0)

ssize_t
krypt_hex_decode(unsigned char *bytes, size_t len, unsigned char **out)
{
    ssize_t ret;
    unsigned char *retval;
    int tmp = 0;
    
    int_hex_decode_tests(bytes, len, tmp);
    if (tmp == -1)
	return -1;

    ret = len / 2;
    retval = ALLOC_N(unsigned char, ret);
    if (!int_hex_decode(bytes, len, retval)) {
	xfree(retval);
	return -1;
    }
    *out = retval;
    return ret;
}

/* Krypt::Hex */

#define int_hex_process(bytes, len, mode, ret)					\
do {										\
    ssize_t result_len;								\
    unsigned char *result;							\
    int tmp = 0;								\
    if (!(bytes))								\
        krypt_error_raise(eKryptHexError, "Bytes null");			\
    if ((mode) == KRYPT_HEX_DECODE) {						\
	int_hex_decode_tests((bytes), (len), tmp);				\
	if (tmp == -1)								\
	    krypt_error_raise(eKryptHexError, "Decoding the value failed");	\
	result_len = (len) / 2;							\
    	result = ALLOCA_N(unsigned char, result_len);				\
	tmp = int_hex_decode((bytes), (len), result);				\
    } else if ((mode) == KRYPT_HEX_ENCODE) {					\
	int_hex_encode_tests((bytes), (len), tmp);				\
	if (tmp == -1)								\
	    krypt_error_raise(eKryptHexError, "Encoding the value failed");	\
	result_len = (len) * 2;							\
	result = ALLOCA_N(unsigned char, result_len);				\
	tmp = int_hex_encode((bytes), (len), result);				\
    } else {									\
	krypt_error_raise(rb_eRuntimeError, "Internal error");			\
    }										\
    if (!tmp)									\
	krypt_error_raise(eKryptHexError, "Processing the hex value failed."); 	\
    (ret) = rb_str_new((const char *) result, result_len);			\
} while (0)

/**
 * call-seq:
 *    Krypt::Hex.decode(data) -> String
 *
 * Decodes a hex-encoded string of +data+, which need not necessarily be
 * a String, but must allow a conversion with to_str.
 */
static VALUE
krypt_hex_module_decode(VALUE self, VALUE data)
{
    VALUE ret;
    unsigned char *bytes;
    size_t len;

    StringValue(data);
    len = (size_t) RSTRING_LEN((data));
    bytes = (unsigned char *) RSTRING_PTR((data));
    int_hex_process(bytes, len, KRYPT_HEX_DECODE, ret);
    return ret;
}

/**
 * call-seq:
 *    Krypt::Hex.encode(data) -> String
 *
 * Encodes +data+, a String, or an object allowing conversion with to_str,
 * in hex encoding. 
 */
static VALUE
krypt_hex_module_encode(VALUE self, VALUE data)
{
    VALUE ret;
    unsigned char *bytes;
    size_t len;

    StringValue(data);
    len = (size_t) RSTRING_LEN((data));
    bytes = (unsigned char *) RSTRING_PTR((data));
    int_hex_process(bytes, len, KRYPT_HEX_ENCODE, ret);
    return ret;
}

/* End Krypt::Hex */

void
Init_krypt_hex(void)
{
#if 0
    mKrypt = rb_define_module("Krypt"); /* Let RDoc know */
#endif

    mKryptHex = rb_define_module_under(mKrypt, "Hex");

    eKryptHexError = rb_define_class_under(mKryptHex, "HexError", eKryptError);

    rb_define_module_function(mKryptHex, "decode", krypt_hex_module_decode, 1);
    rb_define_module_function(mKryptHex, "encode", krypt_hex_module_encode, 1);
}
