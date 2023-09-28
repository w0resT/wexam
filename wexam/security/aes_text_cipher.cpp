#include "aes_text_cipher.h"
#include <openssl/evp.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "aes_text_cipher.h"

AesTextCipher::AesTextCipher(const unsigned char* keyData, const unsigned char* ivData) {
	key = keyData;
	iv = ivData;
}

std::string AesTextCipher::encrypt(const std::string& plaintext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cout << "Error creating encryption context." << std::endl;
        return "";
    }

    // Initialize encryption with AES-128 in CBC mode
    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv) != 1) {
        std::cout << "Encryption initialization failed." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    std::vector<unsigned char> ciphertextBuffer(plaintext.length() + EVP_MAX_BLOCK_LENGTH); // Buffer for encrypted data
    int len;

    // Encrypt the input plaintext
    if (EVP_EncryptUpdate(ctx, ciphertextBuffer.data(), &len,
        reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length()) != 1) {
        std::cout << "Data encryption error." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    int ciphertextLen = len;

    // Finalize the encryption process
    if (EVP_EncryptFinal_ex(ctx, ciphertextBuffer.data() + len, &len) != 1) {
        std::cout << "Error completing encryption." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    ciphertextLen += len;

    EVP_CIPHER_CTX_free(ctx);

    std::string result;
    for (int i = 0; i < ciphertextLen; ++i) {
        result += to_hex(ciphertextBuffer[i]); // Convert encrypted bytes to hexadecimal
    }
    return result;
}

std::string AesTextCipher::decrypt(const std::string& ciphertext) {
    // Check if the ciphertext has a valid format (even number of characters)
    if (ciphertext.length() % 2 != 0) {
        std::cout << "Incorrect ciphertext format." << std::endl;
        return "";
    }

    std::string decodedCiphertext;
    for (size_t i = 0; i < ciphertext.length(); i += 2) {
        // Parse the ciphertext into hexadecimal bytes
        std::string hexByte = ciphertext.substr(i, 2);
        unsigned char byteValue = static_cast<unsigned char>(std::stoi(hexByte, nullptr, 16));
        decodedCiphertext.push_back(byteValue);
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cout << "Error creating decryption context." << std::endl;
        return "";
    }

    // Initialize decryption with AES-128 in CBC mode
    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv) != 1) {
        std::cout << "Decryption initialization failed." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    std::string plaintext(decodedCiphertext.length(), '\0');
    int len;

    // Decrypt the input ciphertext
    if (EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]), &len,
        reinterpret_cast<const unsigned char*>(decodedCiphertext.data()), decodedCiphertext.length()) != 1) {
        std::cout << "Data decryption error." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    int plaintextLen = len;

    // Finalize the decryption process
    if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]) + len, &len) != 1) {
        std::cout << "Error completing decryption." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    plaintextLen += len;
    plaintext.resize(plaintextLen);

    EVP_CIPHER_CTX_free(ctx);

    return plaintext;
}

