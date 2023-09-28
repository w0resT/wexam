#pragma once
#include "../interfaces/security/itextcipher.h"
#include <iostream>

class AesTextCipher : public ITextCipher {
public:
    AesTextCipher(const unsigned char* keyData, const unsigned char* ivData);

    std::string encrypt(const std::string& plaintext) override;
    std::string decrypt(const std::string& ciphertext) override;

private:
    const unsigned char* key;
    const unsigned char* iv;

    std::string to_hex(unsigned char c) {
        char hexChars[] = "0123456789ABCDEF";
        std::string result;
        result += hexChars[c >> 4];
        result += hexChars[c & 0x0F];
        return result;
    }

    unsigned char from_hex(char c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            return c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            return c - 'a' + 10;
        }
        else {
            std::cout << "Invalid character in hexadecimal representation: " << c << std::endl;
            return 0;
        }
    }
};