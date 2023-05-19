#pragma once
#include <stdint.h>
#include <stdlib.h>

class ByteOrderConverter {
public:
    /**
    * Convert a 64-bit unsigned integer from host byte order to big-endian byte order.
    *
    * @param value The 64-bit value to be converted.
    * @return The converted value in big-endian byte order.
    */
    uint64_t convertToBigEndian( uint64_t value ) {
#if defined(WIN32)
        return _byteswap_uint64( value );
#else
        return __builtin_bswap64( value );
#endif
    }
};