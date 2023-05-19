#pragma once
#include <vector>
#include "../interfaces/tsystem/itestmanager.h"
#include "../interfaces/tsystem/itest.h"
#include "../interfaces/tsystem/itestrepository.h"
#include "../interfaces/security/itextcipher.h"

/**
 * TestManager is responsible for importing and exporting tests between files and a test repository.
 * It implements the ITestManager interface.
 */
class TestManager : public ITestManager {
public:
	TestManager() {}
	TestManager( std::unique_ptr<ITextCipher> cipher ) : m_cipher( std::move( cipher ) ) {}

	/**
	 * Import tests from a file and add them to the test repository.
	 *
	 * @param filename The name of the file to import tests from.
	 * @param useDecrypt Flag indicating whether the file is encrypted and needs to be decrypted.
	 * @param repository The test repository to add the tests to.
	 * @param needCorAnswers Whether to include correct answers in the imported tests.
	 * @param needUserAnswers Whether to include user answers in the imported tests.
	 * @throws std::runtime_error if the file fails to open for reading.
	 */
	void ImportTests( const std::string& filename, 
					  std::shared_ptr<ITestRepository> repository, 
					  bool useDecrypt = false, 
					  bool needCorAnswers = false, 
					  bool needUserAnswers = false ) override;

	/**
	 * Export tests from the test repository to a file.
	 *
	 * @param filename The name of the file to export tests to.
	 * @param repository The test repository to export tests from.
	 * @param useEncrypt Flag indicating whether the exported tests should be encrypted
	 * @param needCorAnswers Whether to include correct answers in the exported tests.
	 * @param needUserAnswers Whether to include user answers in the exported tests.
	 * @throws std::runtime_error if the file fails to open for writing.
	 */
	void ExportTests( const std::string& filename, 
					  std::shared_ptr<ITestRepository> repository, 
					  bool useEncrypt = false, 
					  bool needCorAnswers = false, 
					  bool needUserAnswers = false ) override;

private:
	/**
	 * Split a string into substrings based on a delimiter character.
	 *
	 * @param str The string to be split.
	 * @param delimiter The delimiter character to split the string by.
	 * @return A vector of substrings.
	 */
	std::vector<std::string> SplitString( const std::string& str, char delimiter );

	/**
	 * Parse tests from a file stream.
	 *
	 * @param ssTest The stringstream containing the test data to parse.
	 * @param needCorAnswers Whether to include correct answers in the parsed tests.
	 * @param needUserAnswers Whether to include user answers in the parsed tests.
	 * @return A vector of parsed tests.
	 * @throws std::runtime_error if the question type is unknown
	 */
	std::vector<std::shared_ptr<ITest>> ParseTestsFromFile( std::stringstream& issTest, bool needCorAnswers = false, bool needUserAnswers = false );

	/**
	 * Format a test as a string line for exporting.
	 *
	 * @param test The test to be formatted.
	 * @param needCorAnswers Whether to include correct answers in the formatted test.
	 * @param needUserAnswers Whether to include user answers in the formatted test.
	 * @return The formatted test line.
	 */
	std::string FormatTestLine( const std::shared_ptr<ITest>& test, bool needCorAnswers = false, bool needUserAnswers = false );

private:
	std::unique_ptr<ITextCipher> m_cipher;

};