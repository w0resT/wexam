#pragma once
#include <unordered_map>
#include <string>

class ILocalizationWriter {
public:
	virtual void WriteTranslation(const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& translation, const std::string& filename) = 0;

	virtual ~ILocalizationWriter() {}
};