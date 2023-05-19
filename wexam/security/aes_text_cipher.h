#pragma once
#include "../interfaces/security/itextcipher.h"

class AesTextCipher : public ITextCipher {
public:
    AesTextCipher( const std::string& key );

    /**
     *
     * Encrypts the given text using AES encryption algorithm.
     *
     * @param text The text to be encrypted.
     * @return The encrypted text.
     */
    std::string encrypt( const std::string& text ) override;

    /**
     *
     * Decrypts the given encrypted text using AES decryption algorithm.
     *
     * @param encryptedText The encrypted text to be decrypted.
     * @return The decrypted text.
     */
    std::string decrypt( const std::string& encryptedText ) override;

private:
    std::string m_key;
};