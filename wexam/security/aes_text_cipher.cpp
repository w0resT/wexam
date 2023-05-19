#include <openssl/aes.h>
#include <vector>
#include <stdexcept>
#include "aes_text_cipher.h"

// Error C4996 '': Since OpenSSL 3.0
#pragma warning(disable : 4996)

inline AesTextCipher::AesTextCipher( const std::string& key ) : m_key( key ) {
    if ( key.length() != AES_BLOCK_SIZE ) {
        throw std::invalid_argument( "Key length must be 128 bits!" );
    }
}

inline std::string AesTextCipher::encrypt( const std::string& text ) {
    AES_KEY aesKey;
    std::vector<unsigned char> aesKeyBuf( AES_BLOCK_SIZE );
    std::copy( m_key.begin(), m_key.end(), aesKeyBuf.begin() );
    AES_set_encrypt_key( aesKeyBuf.data(), 128, &aesKey );

    std::string encryptedText;
    size_t textLength = text.length();

    for ( size_t i = 0; i < textLength; i += AES_BLOCK_SIZE ) {
        std::string block = text.substr( i, AES_BLOCK_SIZE );
        std::vector<unsigned char> inputBuf( AES_BLOCK_SIZE, 0 );
        std::copy( block.begin(), block.end(), inputBuf.begin() );

        std::vector<unsigned char> outputBuf( AES_BLOCK_SIZE );
        AES_encrypt( inputBuf.data(), outputBuf.data(), &aesKey );

        encryptedText.append( reinterpret_cast< char* >( outputBuf.data() ), AES_BLOCK_SIZE );
    }

    return encryptedText;
}

inline std::string AesTextCipher::decrypt( const std::string& encryptedText ) {
    AES_KEY aesKey;
    std::vector<unsigned char> aesKeyBuf( AES_BLOCK_SIZE );
    std::copy( m_key.begin(), m_key.end(), aesKeyBuf.begin() );
    AES_set_decrypt_key( aesKeyBuf.data(), 128, &aesKey );

    std::string decryptedText;
    size_t textLength = encryptedText.length();

    for ( size_t i = 0; i < textLength; i += AES_BLOCK_SIZE ) {
        std::string block = encryptedText.substr( i, AES_BLOCK_SIZE );
        std::vector<unsigned char> inputBuf( AES_BLOCK_SIZE );
        std::copy( block.begin(), block.end(), inputBuf.begin() );

        std::vector<unsigned char> outputBuf( AES_BLOCK_SIZE );
        AES_decrypt( inputBuf.data(), outputBuf.data(), &aesKey );

        decryptedText.append( reinterpret_cast< char* >( outputBuf.data() ), AES_BLOCK_SIZE );
    }

    return decryptedText;
}
