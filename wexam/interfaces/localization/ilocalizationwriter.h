#pragma once
#include <map>
#include <string>

class ILocalizationWriter {
public:
	virtual void WriteTranslation(const std::map<std::string, std::map<std::string, std::string>>& translation, const std::string& filename) = 0;

	virtual ~ILocalizationWriter() {}
};