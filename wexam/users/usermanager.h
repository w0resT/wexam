#pragma once
#include <vector>
#include "../interfaces/users/iusermanager.h"
#include "../interfaces/users/istudent.h"
#include "../interfaces/users/istudentrepository.h"

class UserManager : public IUserManager {
public:
	UserManager() {}

	/**
	 * Imports users from a file and adds them to the student repository.
	 *
	 * @param filename The name of the file to import from.
	 * @param repository The student repository to add the imported users to.
	 */
	void ImportUsers( const std::string& filename,
					  std::shared_ptr<IStudentRepository> repository ) override;

	/**
	 * Exports users from the student repository to a file.
	 *
	 * @param filename The name of the file to export to.
	 * @param repository The student repository to export users from.
	 */
	void ExportUsers( const std::string& filename,
					  std::shared_ptr<IStudentRepository> repository ) override;

private:
	/**
	 * Parses user data from a string stream and returns a vector of parsed student objects.
	 *
	 * @param issUser The string stream containing the user data.
	 * @return A vector of shared pointers to the parsed student objects.
	 */
	std::vector<std::shared_ptr<IStudent>> ParseUsersFromFile( std::stringstream& issUser);

	/**
	 * Formats a user object as a string line for exporting.
	 *
	 * @param user The student object to format.
	 * @return The formatted string line representing the user.
	 */
	std::string FormatUserLine( const std::shared_ptr<IStudent>& user );
};