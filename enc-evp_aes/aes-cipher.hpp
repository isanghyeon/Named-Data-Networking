//
//  aes-cipher.hpp
//
//  Created by 이상현 on 2022/04/29.
//

#ifndef aes_cipher_hpp
#define aes_cipher_hpp

#include <openssl/bn.h>

namespace ndn {
    namespace security {
        namespace transform {

            /**
             * @brief The module to encrypt data using AES block cipher.
             *
             * The padding scheme of the block cipher is set to the OpenSSL default,
             * which is PKCS padding.
             */

            class AESCipher final : public Transform {
            public:
                /**
                 * @brief Create a AES block cipher.
                 *
                 * @param algo The block cipher algorithm to use.
                 * @param op   The operation to perform (encrypt or decrypt).
                 * @param key  The symmetric key.
                 * @param iv   The initialization vector.
                 */
                AESCipher(span<const uint8_t> P, uint32_t TargetLen, span<const uint8_t> C, span<const uint8_t> K, span<const uint8_t> iv);

                ~AESCipher() final;

            private:
                void str2hex(span<const uint8_t> in, span<const uint8_t> out, uint32_t TargetLen);

            private:
                void handleErrors(void) final;
            };
        } // namespace transform
    } // namespace security
} // namespace ndn


#endif /* aes_cipher_hpp */
