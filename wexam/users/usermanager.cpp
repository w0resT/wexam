#include <fstream>
#include <sstream>
#include <regex>
#include "usermanager.h"
#include "studentrepository.h"
#include "student.h"

void UserManager::ImportUsers( const std::string& filename, std::shared_ptr<IStudentRepository> repository ) {
	// Open the file for reading
	std::ifstream file( filename, std::ios::binary );
	if ( !file.is_open() ) {
		throw std::runtime_error( "Failed to open file for reading: " + filename );
	}

	std::string text;
	std::string encTestLine;

	// Get tests from file
	while ( std::getline( file, encTestLine ) ) {
		text += encTestLine;
		text += "\n";
	}

	// If encrypted then use decrypt overwise just text
	std::stringstream ssText;
	ssText << text;

	// Read test data from the file and add users to the repository
	std::vector<std::shared_ptr<IStudent>> users = ParseUsersFromFile( ssText );
	repository->AddStudents( users );

	// Close the file
	file.close();
}

void UserManager::ExportUsers( const std::string& filename, std::shared_ptr<IStudentRepository> repository ) {
	// Open the file for writing
	std::ofstream file( filename, std::ios::binary );
	if ( !file.is_open() ) {
		throw std::runtime_error( "Failed to open file for writing: " + filename );
	}

	std::string userFull;

	// Iterate over the tests in the repository and write their data to the file
	for ( unsigned int i = 0; i < repository->GetStudents().size(); ++i ) {
		std::shared_ptr<IStudent> user = repository->GetStudent( i );
		std::string userLine = FormatUserLine( user );

		userFull += userLine;
		//testFull += "\n";
	}

	file << userFull;

	// Close the file
	file.close();
}

inline std::vector<std::shared_ptr<IStudent>> UserManager::ParseUsersFromFile( std::stringstream& issUser ) {
	std::vector<std::shared_ptr<IStudent>> users;
	std::shared_ptr<IStudent> user = std::make_shared<Student>();

	// Format:
	// [ID: userId][Name: userName][Group: userGroup]

	std::string userLine;
	while ( std::getline( issUser, userLine ) ) {
		// Parse test ID, name and group
		std::regex idRegex( R"(\[ID:\s*(\d+)\])" );
		std::regex nameRegex( R"(\[Name:\s*(.*?)\])" );
		std::regex groupRegex( R"(\[Group:\s*(.*?)\])" );
		std::smatch match;

		// Looking for ID
		if ( std::regex_search( userLine, match, idRegex ) ) {
			// If id != 0 then it's not a first loop so clear test
			if ( user->GetId() != 0 ) {
				users.emplace_back( user );
				user.reset();
				user = std::make_shared<Student>();
			}

			unsigned int userId = std::stoi( match[ 1 ].str() );
			user->SetId( userId );
		}

		// Looking for Name
		if ( std::regex_search( userLine, match, nameRegex ) ) {
			std::string userName = match[ 1 ].str();
			user->SetName( userName );
		}

		// Looking for Group
		if ( std::regex_search( userLine, match, groupRegex ) ) {
			std::string userGroup = match[ 1 ].str();
			user->SetGroup( userGroup );
		}
	}

	users.emplace_back( user );

	return users;
}

inline std::string UserManager::FormatUserLine( const std::shared_ptr<IStudent>& user ) {
	// Format:
	// [ID: userId][Name: userName][Group: userGroup]

	std::string userLine = "[ID: " + std::to_string( user->GetId() ) + "]";
	userLine += "[Name: " + user->GetName() + "]";
	userLine += "[Group: " + user->GetGroup() + "]\n";

	return userLine;
}

