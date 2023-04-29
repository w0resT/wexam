#include "localization_manager.h"

void LocalizationManager::SetLanguage(const std::string& language) {
	m_currentLanguage = language;
}

std::string LocalizationManager::GetLanguage() const {
	return m_currentLanguage;
}

std::string LocalizationManager::GetTranslation(const std::string& key) const {
	if (m_translations.count(m_currentLanguage) > 0 && m_translations.at(m_currentLanguage).count(key) > 0) {
		return m_translations.at(m_currentLanguage).at(key);
	} 
	else {
		return "ERROR";
	}
}

void LocalizationManager::AddTranslation(const std::string& lang, const std::string& key, const std::string& translation) {
	m_translations[lang][key] = translation;
}

void LocalizationManager::SaveTranslation() {
	m_writer->WriteTranslation(m_translations, m_fileName);
}

void LocalizationManager::LoadTranslation() {
	m_translations = m_reader->ReadTranslation(m_fileName);
}
