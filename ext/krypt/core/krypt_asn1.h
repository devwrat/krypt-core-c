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

#if !defined(_KRYPT_ASN1_H)
#define _KRYPT_ASN1_H_

extern VALUE mAsn1;
extern VALUE cAsn1Parser;
extern VALUE cAsn1Header;

extern VALUE eAsn1Error;
extern VALUE eParseError;
extern VALUE eSerializeError;

typedef struct krypt_asn1_header_st {
    int tag;
    int tag_class;
    int is_constructed;
    int is_infinite;
    int header_len;
    int len;
} krypt_asn1_header;

void krypt_asn1_next_header(krypt_instream *in, krypt_asn1_header *out);

void Init_krypt_asn1(void);
void Init_krypt_asn1_parser(void);

#endif /* _KRYPT_ASN1_H */

