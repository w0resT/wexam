#include "CppUnitTest.h"
#include "../wexam/localization/localization_reader.h"
#include "../wexam/localization/localization_writer.h"
#include <fstream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LocalizationManagerTests {
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

	TEST_CLASS( LocalizationWriterTest ) {
public:

	TEST_METHOD( WriteTranslation_ValidData ) {
		LocalizationWriter writer;

		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translation;
		translation[ "en" ] = { { "greeting", "Hello" }, { "farewell", "Goodbye" } };
		translation[ "fr" ] = { { "greeting", "Bonjour" }, { "farewell", "Au revoir" } };

		std::string filename = "test_translation.txt";

		writer.WriteTranslation( translation, filename );

		// Check if the file was created
		std::ifstream file( filename );
		Assert::IsTrue( file.good() );

		file.close();

		// Clean up the test file
		std::remove( filename.c_str() );
	}

	TEST_METHOD( WriteTranslation_EmptyTranslation ) {
		LocalizationWriter writer;

		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translation;

		std::string filename = "empty_translation.txt";

		Assert::ExpectException<std::logic_error>( [ & ] () {
			writer.WriteTranslation( translation, filename ); } );
	}

	TEST_METHOD( WriteTranslation_FailedToOpenFile ) {
		LocalizationWriter writer;

		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translation;
		translation[ "en" ] = { { "greeting", "Hello" }, { "farewell", "Goodbye" } };

		std::string filename = "nonexistent_directory/test_translation.txt";

		Assert::ExpectException<std::ofstream::failure>( [ & ] () {
			writer.WriteTranslation( translation, filename ); } );
	}

	};
}
