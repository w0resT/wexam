#pragma once
#include "../interfaces/localization/ilocalizationreader.h"

class LocalizationReader : public ILocalizationReader {
public:
	LocalizationReader() {}

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ReadTranslation(const std::string& filename) const override;
};