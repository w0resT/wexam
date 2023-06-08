#include <fstream>
#include <assert.h>
#include "localization_writer.h"

void LocalizationWriter::WriteTranslation(const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& translation, const std::string& filename) {
	std::ofstream file(filename);

	// Expected a non-empty translation
	if ( translation.empty() ) {
		throw std::logic_error( "No translation" );
	}

	// Does the file open
	if ( !file ) {
		throw std::ofstream::failure( "Failed to open file" );
	}

	for (const auto& [lang, table] : translation) {
		file << "[" << lang << "]" << std::endl;

		for (const auto& [key, translate] : table) {
			file << key << "=" << translate << std::endl;
		}
		file << std::endl;
	}

	file.close();
}
