#pragma once
#include "../interfaces/security/itotp.h"

class TOTPGenerator : public ITOTP {
public:
    /**
     * Generate a Time-Based One-Time Password (TOTP) using HMAC-SHA1 algorithm.
     *
     * @param secret The secret key used for generating the TOTP.
     * @param timestamp The current timestamp.
     * @param tokenLength The length of the generated TOTP token (default: 6).
     * @return The generated TOTP as a string.
     */
    std::string Generate( const std::string& secret, uint64_t timestamp, int tokenLength = 6 ) override;
};