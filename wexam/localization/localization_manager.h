#pragma once
#include <memory>

#include "../interfaces/localization/ilocalizationmanager.h"

#include "localization_writer.h"
#include "localization_reader.h"

class LocalizationManager : public ILocalizationManager {
public:
	LocalizationManager(std::unique_ptr<ILocalizationReader> reader, std::unique_ptr<ILocalizationWriter> writer, const std::string& filename) :
		m_reader(std::move(reader)), m_writer(std::move(writer)), m_fileName(filename) {}

	void SetLanguage(const std::string& language) override;
	std::string GetLanguage() const override;

	std::string GetTranslation(const std::string& key) const override;
	void AddTranslation(const std::string& lang, const std::string& key, const std::string& translation) override;

	void SaveTranslation() override;
	void LoadTranslation() override;

private:
	std::map<std::string, std::map<std::string, std::string>> m_translations;
	std::string m_currentLanguage;

	std::unique_ptr<ILocalizationReader> m_reader;
	std::unique_ptr<ILocalizationWriter> m_writer;

	std::string m_fileName;
};