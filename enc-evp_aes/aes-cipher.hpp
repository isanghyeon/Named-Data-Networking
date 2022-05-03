//
//  aes-cipher.hpp
//
//  Created by 이상현 on 2022/04/29.
//

#ifndef NDN_CXX_SECURITY_TRANSFORM_AES_CIPHER_HPP
#define NDN_CXX_SECURITY_TRANSFORM_AES_CIPHER_HPP


#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

void str2hex(unsigned char *in, unsigned char *out, int ciphertext_len);

void handleErrors();

int EVP_AES_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);


#endif /* NDN_CXX_SECURITY_TRANSFORM_AES_CIPHER_HPP */
