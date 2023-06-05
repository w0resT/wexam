#include <fstream>
#include <sstream>
#include <assert.h>
#include "localization_reader.h"

std::unordered_map<std::string, std::unordered_map<std::string, std::string>> LocalizationReader::ReadTranslation(const std::string& filename) const {
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translation;

    std::ifstream file(filename);
    assert(file && "Filed to open file");

    std::string line;
    std::string currentLang;

    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == ';') {
            continue;
        }

        // Check if its a lang section
        if (line[0] == '[') {
            currentLang = line.substr(1, line.size() - 2);
        }
        // Key section
        else {
            std::istringstream ss(line);
            std::string key;
            std::string translate;

            std::getline(std::getline(ss, key, '='), translate);

            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            translate.erase(0, translate.find_first_not_of(" \t"));
            translate.erase(translate.find_last_not_of(" \t") + 1);

            translation[currentLang][key] = translate;
        }
    }

    file.close();

    return translation;
}
