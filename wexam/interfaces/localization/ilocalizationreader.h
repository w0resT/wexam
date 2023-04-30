#pragma once
#include <unordered_map>
#include <string>

class ILocalizationReader {
public:
	virtual std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ReadTranslation(const std::string& filename) const = 0;

	virtual ~ILocalizationReader() {}
};