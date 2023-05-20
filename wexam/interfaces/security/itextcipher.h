#pragma once
#include <string>

class ITextCipher {
public:
	virtual ~ITextCipher() {}
	virtual std::string encrypt( const std::string& text ) = 0;
	virtual std::string decrypt( const std::string& encryptedText ) = 0;
};