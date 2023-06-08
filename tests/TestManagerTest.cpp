#include <fstream>
#include "CppUnitTest.h"
#include "../wexam/tsystem/testmanager.h"
#include "../wexam/tsystem/testrepository.h"
#include "../wexam/tsystem/test.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestManagerTests {
    TEST_CLASS( ImportTestsTests ) {
public:
    TEST_METHOD( ImportTests_ValidData ) {
        TestManager testManager;
        std::shared_ptr<ITestRepository> repository = std::make_shared<TestRepository>();

        // Set up a temporary file with some test data
        std::string testFilePath = "test_tests.txt";
        std::ofstream file( testFilePath );
        file << "[ID: 1][Title: Test 1][Description: Description 1]\n";
        file << "1. [2] [Question 1] : [Option 1, Option 2], [Option 1], [Option 1]\n";
        file << "2. [1] [Question 2] : [], [Answer 2], [User Answer 2]\n";
        file.close();

        // Call the ImportTests function
        testManager.ImportTests( testFilePath, repository, false, true, true );

        // Check that the tests have been added to the repository correctly
        Assert::IsNotNull( repository->FindTestById( 1 ).get() );
        Assert::IsNotNull( repository->FindTestById( 1 )->FindQuestionById( 1 ).get() );
        Assert::IsNotNull( repository->FindTestById( 1 )->FindQuestionById( 2 ).get() );
        Assert::IsTrue( repository->FindTestById( 1 )->GetTitle() == "Test 1" );
        Assert::IsTrue( repository->FindTestById( 1 )->GetDescription() == "Description 1" );
        Assert::IsTrue( repository->FindTestById( 1 )->FindQuestionById( 1 )->GetQuestionType() == QuestionType::AnswerOptions );
        Assert::IsTrue( repository->FindTestById( 1 )->FindQuestionById( 2 )->GetQuestionType() == QuestionType::FreeAnswer );
        Assert::IsTrue( repository->FindTestById( 1 )->FindQuestionById( 1 )->GetQuestion() == "Question 1");
        Assert::IsTrue( repository->FindTestById( 1 )->FindQuestionById( 2 )->GetQuestion() == "Question 2" );
        Assert::IsTrue( repository->FindTestById( 1 )->FindQuestionById( 1 )->GetCorrectAnswer() == "Option 1");
        Assert::IsTrue( repository->FindTestById( 1 )->FindQuestionById( 2 )->GetCorrectAnswer() == "Answer 2" );
        Assert::IsTrue( repository->FindTestById( 1 )->FindQuestionById( 1 )->GetUserAnswer() == "Option 1" );
        Assert::IsTrue( repository->FindTestById( 1 )->FindQuestionById( 2 )->GetUserAnswer() == "User Answer 2" );

        // Clean up the temporary file
        std::remove( testFilePath.c_str() );
    }

    TEST_METHOD( ImportTests_FileOpenFailed ) {
        TestManager testManager;
        std::shared_ptr<ITestRepository> repository = std::make_shared<TestRepository>();

        // Set up a non-existent file path for testing
        std::string testFilePath = "nonexistent_directory/test_tests.txt";

        // Call the ImportTests function and check for the expected exception
        Assert::ExpectException<std::runtime_error>( [ & ] () {
            testManager.ImportTests( testFilePath, repository, false, true, true ); } );
    }
    };

    TEST_CLASS( ExportTestsTests ) {
public:
    TEST_METHOD( ExportTests_ValidData ) {
        TestManager testManager;
        std::shared_ptr<ITestRepository> repository = std::make_shared<TestRepository>();

        // Set up a temporary file path for testing
        std::string testFilePath = "test_tests.txt";

        // Create a test
        std::shared_ptr<ITest> test = std::make_shared<Test>( 1, "Math Test", "Test on basic math concepts" );

        // Add questions to the test
        test->AddQuestion( 1, "What is 2 + 2?", { "3", "4", "5" }, "4" );
        test->AddQuestion( 2, "What is the square root of 16?", { "2", "4", "8" }, "4" );
        test->AddQuestion( 3, "Free answer test?", "yes" );

        // Add a test to the repository
        repository->AddTest( test );

        // Call the ExportTests function
        testManager.ExportTests( testFilePath, repository, false, true, true );

        // Check that the file has been created
        std::ifstream file( testFilePath );
        Assert::IsTrue( file.good() );

        // Close and remove the temporary file
        file.close();
        std::remove( testFilePath.c_str() );
    }

    TEST_METHOD( ExportTests_FileOpenFailed ) {
        TestManager testManager;
        std::shared_ptr<ITestRepository> repository = std::make_shared<TestRepository>();

        // Set up a non-existent file path for testing
        std::string testFilePath = "nonexistent_directory/test_tests.txt";

        // Call the ExportTests function and check for the expected exception
        Assert::ExpectException<std::runtime_error>( [ & ] () {
            testManager.ExportTests( testFilePath, repository, false, true, true ); } );
    }
    };
}