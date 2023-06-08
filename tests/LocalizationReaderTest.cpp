#include "CppUnitTest.h"
#include "../wexam/localization/localization_reader.h"
#include <fstream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LocalizationReaderTests {
	TEST_CLASS( LocalizationReaderTest ) {
public:

	TEST_METHOD( ReadTranslation_ValidFile ) {
		LocalizationReader reader;
		std::string filename = "valid_file.txt";
		std::ofstream file( filename );
		file << "[en]" << std::endl;
		file << "greeting=Hello" << std::endl;
		file << "farewell=Goodbye" << std::endl;
		file << std::endl;
		file << "[fr]" << std::endl;
		file << "greeting=Bonjour" << std::endl;
		file << "farewell=Au revoir" << std::endl;
		file.close();

		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> expectedTranslation;
		expectedTranslation[ "en" ] = { { "greeting", "Hello" }, { "farewell", "Goodbye" } };
		expectedTranslation[ "fr" ] = { { "greeting", "Bonjour" }, { "farewell", "Au revoir" } };

		auto translation = reader.ReadTranslation( filename );

		// Clean up the test file
		std::remove( filename.c_str() );

		Assert::IsTrue( translation == expectedTranslation );
	}

	TEST_METHOD( ReadTranslation_EmptyFile ) {
		LocalizationReader reader;
		std::string filename = "empty_file.txt";
		std::ofstream file( filename );
		file.close();

		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> expectedTranslation;

		auto translation = reader.ReadTranslation( filename );

		// Clean up the test file
		std::remove( filename.c_str() );

		Assert::IsTrue( translation == expectedTranslation );
	}

	TEST_METHOD( ReadTranslation_FileNotFound ) {
		LocalizationReader reader;
		std::string filename = "nonexistent_file.txt";

		Assert::ExpectException<std::ofstream::failure>( [ & ] () {
			reader.ReadTranslation( filename ); } );
	}

	};
}
