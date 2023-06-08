#include <fstream>
#include "CppUnitTest.h"
#include "../wexam/users/usermanager.h"
#include "../wexam/users/studentrepository.h"
#include "../wexam/users/student.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UserManagerTests {
    TEST_CLASS( ImportUsersTests ) {
public:
    TEST_METHOD( ImportUsers_ValidData ) {
        UserManager userManager;
        std::shared_ptr<IStudentRepository> repository = std::make_shared<StudentRepository>();

        // Set up a temporary file with some test data
        std::string testFilePath = "test_users.txt";
        std::ofstream file( testFilePath );
        file << "[ID: 1][Name: John][Group: A]\n";
        file << "[ID: 2][Name: Jane][Group: B]\n";
        file.close();

        userManager.ImportUsers( testFilePath, repository );

        // Check that the users have been added to the repository correctly
        Assert::IsNotNull( repository->FindStudentById( 1 ).get() );
        Assert::IsNotNull( repository->FindStudentById( 2 ).get() );
        Assert::IsTrue( repository->FindStudentById( 1 )->GetName() == "John" );
        Assert::IsTrue( repository->FindStudentById( 2 )->GetName() == "Jane" );
        Assert::IsTrue( repository->FindStudentById( 1 )->GetGroup() == "A" );
        Assert::IsTrue( repository->FindStudentById( 2 )->GetGroup() == "B" );

        // Clean up the temporary file
        std::remove( testFilePath.c_str() );
    }

    TEST_METHOD( ImportUsers_FileOpenFailed ) {
        UserManager userManager;
        std::shared_ptr<IStudentRepository> repository = std::make_shared<StudentRepository>();

        // Set up a non-existent file path for testing
        std::string testFilePath = "nonexistent_directory/test_users.txt";

        // Call the ImportUsers function and check for the expected exception
        Assert::ExpectException<std::runtime_error>( [ & ] () {
            userManager.ImportUsers( testFilePath, repository ); } );
    }
    };

    TEST_CLASS( ExportUsersTests ) {
public:
    TEST_METHOD( ExportUsers_ValidData ) {
        UserManager userManager;
        std::shared_ptr<IStudentRepository> repository = std::make_shared<StudentRepository>();

        // Set up a temporary file path for testing
        std::string testFilePath = "test_users.txt";

        // Add students to the repository
        repository->AddStudent( std::make_shared<Student>( 1, "John", "A" ) );
        repository->AddStudent( std::make_shared<Student>( 2, "Maria", "B" ) );
        repository->AddStudent( std::make_shared<Student>( 3, "Sveta", "C" ) );

        userManager.ExportUsers( testFilePath, repository );

        // Check that the file has been created
        std::ifstream file( testFilePath );
        Assert::IsTrue( file.good() );

        // Clean up the temporary file
        file.close();
        std::remove( testFilePath.c_str() );
    }

    TEST_METHOD( ExportUsers_FileOpenFailed ) {
        UserManager userManager;
        std::shared_ptr<IStudentRepository> repository = std::make_shared<StudentRepository>();

        // Set up a non-existent file path for testing
        std::string testFilePath = "nonexistent_directory/test_users.txt";

        // Call the ExportUsers function and check for the expected exception
        Assert::ExpectException<std::runtime_error>( [ & ] () {
            userManager.ExportUsers( testFilePath, repository ); } );
    }
    };
}
