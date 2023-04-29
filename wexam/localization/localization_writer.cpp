#include <fstream>
#include "localization_writer.h"

void LocalizationWriter::WriteTranslation(const std::map<std::string, std::map<std::string, std::string>>& translation, const std::string& filename) {
	std::ofstream file(filename);
	if (!file) {
		throw std::runtime_error("Failed to open file");
	}

	for (const auto& [lang, table] : translation) {
		file << "[" << lang << "]" << std::endl;

		for (const auto& [key, translate] : table) {
			file << key << "=" << translate << std::endl;
		}
		file << std::endl;
	}
}
