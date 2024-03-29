#pragma once
#include "../interfaces/localization/ilocalizationwriter.h"

class LocalizationWriter : public ILocalizationWriter {
public:
	LocalizationWriter() {}

	void WriteTranslation(const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& translation, const std::string& filename) override;

};