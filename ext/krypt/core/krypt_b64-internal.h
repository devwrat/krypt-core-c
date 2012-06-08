/*
* krypt-core API - C version
*
* Copyright (C) 2012
* Hiroshi Nakamura <nahi@ruby-lang.org>
* Martin Bosslet <martin.bosslet@googlemail.com>
* All rights reserved.
*
* This software is distributed under the same license as Ruby.
* See the file 'LICENSE' for further details.
*/

#ifndef _KRYPT_B64_INTERNAL_H
#define _KRYPT_B64_INTERNAL_H_

ssize_t krypt_base64_encode(unsigned char *bytes, size_t len, int cols, unsigned char **out);
int krypt_base64_buffer_encode_to(krypt_outstream *out, unsigned char *bytes, size_t off, size_t len, int cols);
ssize_t krypt_base64_decode(unsigned char *bytes, size_t len, unsigned char **out);
int krypt_base64_buffer_decode_to(krypt_outstream *out, unsigned char *bytes, size_t off, size_t len);

#endif /* _KRYPT_B64_INTERNAL_H_ */

