#pragma once
#include <string>
#include <bitset>

class Base32Encoder {
public:
    /**
    * Convert a string to Base32 encoding.
    *
    * @param input The input string to be converted.
    * @return The Base32 encoded string.
     */
    std::string stringToBase32( const std::string& input ) {
        const std::string base32Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
        std::string result;

        // Convert each character in the input string to its binary representation
        std::string binaryString;
        for ( char c : input ) {
            std::bitset<8> binary( c );
            binaryString += binary.to_string();
        }

        // Split the binary string into groups of 5 bits and convert them to base32 characters
        for ( size_t i = 0; i < binaryString.size(); i += 5 ) {
            std::string bits = binaryString.substr( i, 5 );
            int index = std::stoi( bits, nullptr, 2 );
            result += base32Chars[ index ];
        }

        // Add padding characters if needed
        size_t padding = 8 - ( result.size() % 8 );
        if ( padding != 8 ) {
            result.append( padding, '=' );
        }

        return result;
    }
};