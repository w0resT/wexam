#pragma once
#include <string>

class ITOTP {
public:
	virtual ~ITOTP() {}

	virtual std::string Generate( const std::string& secret, uint64_t timestamp, int tokenLength = 6 ) = 0;
};