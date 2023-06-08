#include "CppUnitTest.h"
#include "../wexam/users/studentrepository.h"
#include "../wexam/users/student.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StudentRepositoryTests {
    TEST_CLASS( AddStudentTest ) {
public:
    TEST_METHOD( AddStudent_ValidStudent ) {
        StudentRepository repository;
        std::shared_ptr<IStudent> student = std::make_shared<Student>();

        repository.AddStudent( student );

        // Check that the repository contains the added student
        auto retrievedStudent = repository.GetStudent( 0 );
        Assert::IsTrue( student.get() == retrievedStudent.get() );
    }

    TEST_METHOD( AddStudent_NullStudent ) {
        StudentRepository repository;
        std::shared_ptr<IStudent> student = nullptr;

        Assert::ExpectException<std::invalid_argument>( [ & ] () {
            repository.AddStudent( student ); } );
    }

    TEST_METHOD( AddStudent_DuplicateStudent ) {
        StudentRepository repository;
        std::shared_ptr<IStudent> student1 = std::make_shared<Student>();
        std::shared_ptr<IStudent> student2 = std::make_shared<Student>();

        // Add the first student
        repository.AddStudent( student1 );

        // Adding the second student with the same ID should throw an exception
        Assert::ExpectException<std::runtime_error>( [ & ] () {
            repository.AddStudent( student2 ); } );
    }
    };

    TEST_CLASS( AddStudentsTest ) {
public:
    TEST_METHOD( AddStudents_ValidStudents ) {
        StudentRepository repository;
        std::vector<std::shared_ptr<IStudent>> students{
            std::make_shared<Student>(),
            std::make_shared<Student>(),
            std::make_shared<Student>()
        };

        repository.AddStudents( students );

        // Check that the repository contains the added students
        for ( size_t i = 0; i < students.size(); ++i ) {
            auto retrievedStudent = repository.GetStudent( i );
            Assert::IsTrue( students[ i ].get() == retrievedStudent.get() );
        }
    }

    TEST_METHOD( AddStudents_NullStudent ) {
        StudentRepository repository;
        std::vector<std::shared_ptr<IStudent>> students{
            std::make_shared<Student>(),
            nullptr,
            std::make_shared<Student>()
        };

        Assert::ExpectException<std::invalid_argument>( [ & ] () {
            repository.AddStudents( students ); } );
    }
    };

    TEST_CLASS( RemoveStudentTest ) {
public:
    TEST_METHOD( RemoveStudent_ValidId ) {
        StudentRepository repository;
        std::shared_ptr<IStudent> student1 = std::make_shared<Student>( 0, "nameOne", "group" );
        std::shared_ptr<IStudent> student2 = std::make_shared<Student>( 1, "nameTwo", "group" );
        std::shared_ptr<IStudent> student3 = std::make_shared<Student>( 2, "nameThree", "group" );

        // Add the students to the repository
        repository.AddStudent( student1 );
        repository.AddStudent( student2 );
        repository.AddStudent( student3 );
        repository.RemoveStudent( student2->GetId() );

        // Check that the removed student is no longer in the repository
        Assert::AreEqual( 2u, repository.GetStudents().size() );
        Assert::IsTrue( student1.get() == repository.GetStudent( 0 ).get() );
        Assert::IsTrue( student3.get() == repository.GetStudent( 1 ).get() );
    }

    TEST_METHOD( RemoveStudent_InvalidId ) {
        StudentRepository repository;
        std::shared_ptr<IStudent> student1 = std::make_shared<Student>( 0, "nameOne", "group" );
        std::shared_ptr<IStudent> student2 = std::make_shared<Student>( 1, "nameTwo", "group" );
        std::shared_ptr<IStudent> student3 = std::make_shared<Student>( 2, "nameThree", "group" );

        // Add the students to the repository
        repository.AddStudent( student1 );
        repository.AddStudent( student2 );
        repository.AddStudent( student3 );

        unsigned int invalidId = 123;

        repository.RemoveStudent( invalidId );

        // Check that no student was removed
        Assert::AreEqual( 3u, repository.GetStudents().size() );
    }

    TEST_METHOD( RemoveStudent_EmptyRepository ) {
        StudentRepository repository;

        Assert::ExpectException<std::logic_error>( [ & ] () {
            repository.RemoveStudent( 123 ); } );
    }
    };

    TEST_CLASS( GetStudentTest ) {
    public:
        TEST_METHOD( GetStudent_ValidIndex ) {
            StudentRepository repository;
            std::vector<std::shared_ptr<IStudent>> students = {
                std::make_shared<Student>(),
                std::make_shared<Student>(),
                std::make_shared<Student>()
            };
            repository.AddStudents( students );

            unsigned int validIndex = 1;
            auto student = repository.GetStudent( validIndex );

            // Check that the returned student is not null
            Assert::IsNotNull( student.get() );
        }

        TEST_METHOD( GetStudent_InvalidIndex ) {
            StudentRepository repository;
            std::vector<std::shared_ptr<IStudent>> students = {
                std::make_shared<Student>(),
                std::make_shared<Student>(),
                std::make_shared<Student>()
            };
            repository.AddStudents( students );

            unsigned int invalidIndex = 3;

            Assert::ExpectException<std::out_of_range>( [ & ] () {
                repository.GetStudent( invalidIndex ); } );
        }

        TEST_METHOD( GetStudent_EmptyRepository ) {
            StudentRepository repository;

            Assert::ExpectException<std::logic_error>( [ & ] () {
                repository.GetStudent( 0 ); } );
        }
    };

    TEST_CLASS( GetStudentsTest ) {
    public:
        TEST_METHOD( GetStudents_ReturnsCorrectStudents ) {
            StudentRepository repository;
            std::shared_ptr<IStudent> student1 = std::make_shared<Student>( 0, "nameOne", "group" );
            std::shared_ptr<IStudent> student2 = std::make_shared<Student>( 1, "nameTwo", "group" );
            std::shared_ptr<IStudent> student3 = std::make_shared<Student>( 2, "nameThree", "group" );

            // Add the students to the repository
            repository.AddStudent( student1 );
            repository.AddStudent( student2 );
            repository.AddStudent( student3 );

            // Call the GetStudents function
            std::vector<std::shared_ptr<IStudent>> students = repository.GetStudents();

            // Check that the returned vector contains the correct students
            Assert::AreEqual( 3u, students.size() );
            Assert::IsTrue( student1.get() == students[ 0 ].get() );
            Assert::IsTrue( student2.get() == students[ 1 ].get() );
            Assert::IsTrue( student3.get() == students[ 2 ].get() );
        }

        TEST_METHOD( GetStudents_ReturnsEmptyVector ) {
            StudentRepository repository;

            // Call the GetStudents function on an empty repository
            std::vector<std::shared_ptr<IStudent>> students = repository.GetStudents();

            // Check that the returned vector is empty
            Assert::AreEqual( 0u, students.size() );
        }
    };

    TEST_CLASS( FindStudentTest ) {
public:
    TEST_METHOD( FindStudentById_ValidId ) {
        StudentRepository repository;
        std::shared_ptr<IStudent> student1 = std::make_shared<Student>();
        std::shared_ptr<IStudent> student2 = std::make_shared<Student>();
        std::shared_ptr<IStudent> student3 = std::make_shared<Student>();

        // Set unique IDs for each student
        student1->SetId( 1 );
        student2->SetId( 2 );
        student3->SetId( 3 );

        // Add the students to the repository
        repository.AddStudent( student1 );
        repository.AddStudent( student2 );
        repository.AddStudent( student3 );

        // Call the FindStudentById function with a valid ID
        std::shared_ptr<IStudent> foundStudent = repository.FindStudentById( 2 );

        // Check that the correct student is found
        Assert::IsNotNull( foundStudent.get() );
        Assert::IsTrue( student2.get() == foundStudent.get() );
    }

    TEST_METHOD( FindStudentById_InvalidId ) {
        StudentRepository repository;
        std::shared_ptr<IStudent> student1 = std::make_shared<Student>();
        std::shared_ptr<IStudent> student2 = std::make_shared<Student>();
        std::shared_ptr<IStudent> student3 = std::make_shared<Student>();

        // Set unique IDs for each student
        student1->SetId( 1 );
        student2->SetId( 2 );
        student3->SetId( 3 );

        // Add the students to the repository
        repository.AddStudent( student1 );
        repository.AddStudent( student2 );
        repository.AddStudent( student3 );

        // Call the FindStudentById function with an invalid ID
        std::shared_ptr<IStudent> foundStudent = repository.FindStudentById( 4 );

        // Check that no student is found (nullptr is returned)
        Assert::IsNull( foundStudent.get() );
    }

    TEST_METHOD( FindStudentById_EmptyRepository ) {
        StudentRepository repository;

        // Call the FindStudentById function on an empty repository
        Assert::ExpectException<std::logic_error>( [ & ] () {
            repository.FindStudentById( 1 ); } );
    }

    TEST_METHOD( FindStudentByName_ValidName ) {
        StudentRepository repository;
        std::shared_ptr<IStudent> student1 = std::make_shared<Student>();
        std::shared_ptr<IStudent> student2 = std::make_shared<Student>();
        std::shared_ptr<IStudent> student3 = std::make_shared<Student>();

        // Set unique IDs for each student
        student1->SetId( 1 );
        student2->SetId( 2 );
        student3->SetId( 3 );

        // Set unique names for each student
        student1->SetName( "John Doe" );
        student2->SetName( "Jane Smith" );
        student3->SetName( "Michael Johnson" );

        // Add the students to the repository
        repository.AddStudent( student1 );
        repository.AddStudent( student2 );
        repository.AddStudent( student3 );

        // Call the FindStudentByName function with a valid name
        std::shared_ptr<IStudent> foundStudent = repository.FindStudentByName( "Jane Smith" );

        // Check that the correct student is found
        Assert::IsNotNull( foundStudent.get() );
        Assert::IsTrue( student2.get() == foundStudent.get() );
    }

    TEST_METHOD( FindStudentByName_InvalidName ) {
        StudentRepository repository;
        std::shared_ptr<IStudent> student1 = std::make_shared<Student>();
        std::shared_ptr<IStudent> student2 = std::make_shared<Student>();
        std::shared_ptr<IStudent> student3 = std::make_shared<Student>();

        // Set unique IDs for each student
        student1->SetId( 1 );
        student2->SetId( 2 );
        student3->SetId( 3 );

        // Set unique names for each student
        student1->SetName( "John Doe" );
        student2->SetName( "Jane Smith" );
        student3->SetName( "Michael Johnson" );

        // Add the students to the repository
        repository.AddStudent( student1 );
        repository.AddStudent( student2 );
        repository.AddStudent( student3 );

        // Call the FindStudentByName function with an invalid name
        std::shared_ptr<IStudent> foundStudent = repository.FindStudentByName( "John Smith" );

        // Check that no student is found (nullptr is returned)
        Assert::IsNull( foundStudent.get() );
    }

    TEST_METHOD( FindStudentByName_EmptyRepository ) {
        StudentRepository repository;

        // Call the FindStudentByName function on an empty repository
        Assert::ExpectException<std::logic_error>( [ & ] () {
            repository.FindStudentByName( "John Doe" ); } );
    }
    };
}
