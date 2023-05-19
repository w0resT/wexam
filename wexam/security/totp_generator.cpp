#include <openssl/hmac.h>
#include <openssl/sha.h>
#include "totp_generator.h"

inline std::string TOTPGenerator::generateTOTP( const std::string& secret, uint64_t timestamp, int tokenLength ) {
    // Create the HMAC-SHA1 hash
    unsigned char hmacResult[ SHA_DIGEST_LENGTH ];
    unsigned int hmacLen;
    HMAC( EVP_sha1(), secret.c_str(), secret.length(), reinterpret_cast< const unsigned char* >( &timestamp ), sizeof( timestamp ), hmacResult, &hmacLen );

    // Get the offset into the HMAC result
    int offset = hmacResult[ SHA_DIGEST_LENGTH - 1 ] & 0xf;

    // Extract 4 bytes from the hash starting at the offset
    uint32_t truncatedHash =
        ( ( hmacResult[ offset ] & 0x7f ) << 24 ) |
        ( ( hmacResult[ offset + 1 ] & 0xff ) << 16 ) |
        ( ( hmacResult[ offset + 2 ] & 0xff ) << 8 ) |
        ( hmacResult[ offset + 3 ] & 0xff );

    // Apply the modulo operator to truncate the hash
    uint32_t token = truncatedHash % static_cast< uint32_t >( pow( 10, tokenLength ) );

    // Convert the token to a string and pad it with leading zeros if necessary
    std::string totp = std::to_string( token );
    while ( totp.length() < tokenLength ) {
        totp = "0" + totp;
    }

    return totp;
}
