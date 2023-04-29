#pragma once
#include "../interfaces/localization/ilocalizationreader.h"

class LocalizationReader : public ILocalizationReader {
public:
	LocalizationReader() {}

	std::map<std::string, std::map<std::string, std::string>> ReadTranslation(const std::string& filename) const override;
};