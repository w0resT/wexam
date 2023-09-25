#include <fstream>
#include <sstream>
#include <regex>
#include "testmanager.h"
#include "test.h"
#include "question_with_free_answer.h"
#include "question_with_options.h"

void TestManager::ImportTests( const std::string& filename, std::shared_ptr<ITestRepository> repository, bool useDecrypt, bool needCorAnswers, bool needUserAnswers ) {
	// Open the file for reading
	std::ifstream file( filename, std::ios::binary );
	if ( !file.is_open() ) {
		throw std::runtime_error( "Failed to open file for reading: " + filename );
	}

	// Check for m_cipher nullptr
	if ( useDecrypt && !m_cipher ) {
		throw std::runtime_error( "The cipher object is undefined. Forgot the constructor?" );
	}

	std::string text;
	std::string encTestLine;

	// Get tests from file
	while ( std::getline( file, encTestLine ) ) {
		text += encTestLine;
		if ( !useDecrypt ) {
			text += "\n";
		}
	}

	// If encrypted then use decrypt overwise just text
	std::stringstream ssText;
	if ( useDecrypt ) {
		ssText << m_cipher->decrypt( text );
	}
	else {
		ssText << text;
	}

	// Read test data from the file and add tests to the repository
	std::vector<std::shared_ptr<ITest>> tests = ParseTestsFromFile( ssText, needCorAnswers, needUserAnswers );
	repository->AddTests( tests );

	// Close the file
	file.close();
}

void TestManager::ExportTests( const std::string& filename, std::shared_ptr<ITestRepository> repository, bool useEncrypt, bool needCorAnswers, bool needUserAnswers ) {
	// Open the file for writing
	std::ofstream file( filename, std::ios::binary );
	if ( !file.is_open() ) {
		throw std::runtime_error( "Failed to open file for writing: " + filename );
	}

	// Check for m_cipher nullptr
	if ( useEncrypt && !m_cipher ) {
		throw std::runtime_error( "The cipher object is undefined. Forgot the constructor?" );
	}

	std::string testFull;

	// Iterate over the tests in the repository and write their data to the file
	for ( unsigned int i = 0; i < repository->GetTestCount(); ++i ) {
		std::shared_ptr<ITest> test = repository->GetTest( i );
		std::string testLine = FormatTestLine( test, needCorAnswers, needUserAnswers );

		testFull += testLine;
		//testFull += "\n";
	}

	// Encrypt if need
	if ( useEncrypt ) {
		file << m_cipher->encrypt( testFull );
	}
	else {
		file << testFull;
	}

	// Close the file
	file.close();
}

inline std::vector<std::string> TestManager::SplitString( const std::string& str, char delimiter ) {
	std::vector<std::string> substrings;
	std::stringstream ss( str );
	std::string token;

	while ( std::getline( ss, token, delimiter ) ) {
		substrings.push_back( token );
	}

	return substrings;
}

inline std::vector<std::shared_ptr<ITest>> TestManager::ParseTestsFromFile( std::stringstream& issTest, bool needCorAnswers, bool needUserAnswers ) {
	std::vector<std::shared_ptr<ITest>> tests;
	std::shared_ptr<ITest> test = std::make_shared<Test>();

	std::string testLine;
	while ( std::getline( issTest, testLine ) ) {
		// Parse test ID, title, and description
		std::regex idRegex( R"(\[ID:\s*(\d+)\])" );
		std::regex titleRegex( R"(\[Title:\s*(.*?)\])" );
		std::regex descriptionRegex( R"(\[Description:\s*(.*?)\])" );
		std::smatch match;

		// Looking for ID
		if ( std::regex_search( testLine, match, idRegex ) ) {
			// If id != 0 then it's not a first loop so clear test
			if ( test->GetId() != 0 ) {
				tests.emplace_back( test );
				test.reset();
				test = std::make_shared<Test>();
			}

			unsigned int testId = std::stoi( match[ 1 ].str() );
			test->SetId( testId );
		}

		// Looking for Title
		if ( std::regex_search( testLine, match, titleRegex ) ) {
			std::string testTitle = match[ 1 ].str();
			test->SetTitle( testTitle );
		}

		// Looking for Description
		if ( std::regex_search( testLine, match, descriptionRegex ) ) {
			std::string testDescription = match[ 1 ].str();
			test->SetDescription( testDescription );
		}

		// Parse questions
		// Format:
		// questionId. [questionType] [questionText] : [answerOptions], [correctAnswer], [userAnswer]
		std::regex questionRegex( R"((\d+)\.\s+\[(.*?)\]\s+\[(.*?)\]\s+:\s+\[(.*?)\],\s+\[(.*?)\],\s+\[(.*?)\])" );
		std::sregex_iterator questionIterator(testLine.begin(), testLine.end(), questionRegex );
		std::sregex_iterator end;

		for ( ; questionIterator != end; ++questionIterator ) {
			unsigned int questionId = std::stoi( ( *questionIterator )[ 1 ].str() );
			QuestionType questionType = static_cast< QuestionType >( std::stoi( ( *questionIterator )[ 2 ].str() ) );
			std::string questionText = ( *questionIterator )[ 3 ].str();
			std::string answerOptions = ( *questionIterator )[ 4 ].str();
			std::string correctAnswer = ( *questionIterator )[ 5 ].str();
			std::string userAnswer = ( *questionIterator )[ 6 ].str();

			std::shared_ptr<IQuestion> question;

			if ( questionType == QuestionType::FreeAnswer ) {
				question = std::make_shared<QuestionWithFreeAnswer>();
			}
			else if ( questionType == QuestionType::AnswerOptions ) {
				question = std::make_shared<QuestionWithAnswerOptions>();
			}
			else {
				throw std::runtime_error( "The question type is unknown: " + std::to_string( static_cast< int >( questionType ) ) );
			}

			question->SetId( questionId );
			question->SetQuestion( questionText );

			if ( questionType == QuestionType::AnswerOptions ) {
				question->SetAnswerOptions( SplitString( answerOptions, '\\' ) );
			}

			if ( needCorAnswers ) {
				question->SetCorrectAnswer( correctAnswer );
			}

			if ( needUserAnswers ) {
				question->SetUserAnswer( userAnswer );
			}

			test->AddQuestion( question );
		}
	}

	tests.emplace_back( test );

	return tests;
}

inline std::string TestManager::FormatTestLine( const std::shared_ptr<ITest>& test, bool needCorAnswers, bool needUserAnswers ) {
	std::string testLine = "[ID: " + std::to_string( test->GetId() ) + "]";
	testLine += "[Title: " + test->GetTitle() + "]";
	testLine += "[Description: " + test->GetDescription() + "]\n";

	// Format:
	// questionId. [questionType] [questionText] : [answerOptions], [correctAnswer], [userAnswer]
	for ( unsigned int i = 0; i < test->GetQuestionCount(); ++i ) {
		std::shared_ptr<IQuestion> question = test->GetQuestion( i );
		testLine += std::to_string( question->GetId() ) + ". ";
		testLine += "[" + std::to_string( static_cast< int >( question->GetQuestionType() ) ) + "] ";
		testLine += "[" + question->GetQuestion() + "] : [";

		auto questionType = question->GetQuestionType();
		if ( questionType == QuestionType::AnswerOptions ) {
			std::vector<std::string> answerOptions = question->GetAnswerOptions();
			for ( size_t j = 0; j < answerOptions.size(); ++j ) {
				testLine += answerOptions[ j ];
				if ( j < answerOptions.size() - 1 ) {
					testLine += "\\";
				}
			}
		}

		testLine += "], [";
		if ( needCorAnswers ) {
			testLine += question->GetCorrectAnswer();
		}

		testLine += "], [";
		if ( needUserAnswers ) {
			testLine += question->GetUserAnswer();
		}

		testLine += "]\n";
	}

	return testLine;
}
