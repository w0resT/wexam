#pragma once
#include <string>

class ILocalizationManager {
public:
	virtual void SetLanguage(const std::string& language) = 0;
	virtual std::string GetLanguage() const = 0;

	virtual std::string GetTranslation(const std::string& key) const = 0;
	virtual void AddTranslation(const std::string& lang, const std::string& key, const std::string& translation) = 0;

	virtual void SaveTranslation() = 0;
	virtual void LoadTranslation() = 0;

	virtual ~ILocalizationManager() {}
};