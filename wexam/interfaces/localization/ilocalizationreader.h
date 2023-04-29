#pragma once
#include <map>
#include <string>

class ILocalizationReader {
public:
	virtual std::map<std::string, std::map<std::string, std::string>> ReadTranslation(const std::string& filename) const = 0;

	virtual ~ILocalizationReader() {}
};