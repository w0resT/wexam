#pragma once
#include <memory>
#include <string>

class IStudentRepository;

class IUserManager {
public:
	virtual ~IUserManager() {}

	// Import users from a file into the repository
	virtual void ImportUsers( const std::string& filename,
							  std::shared_ptr<IStudentRepository> repository ) = 0;

	// Export users from the repository to a file
	virtual void ExportUsers( const std::string& filename,
							  std::shared_ptr<IStudentRepository> repository ) = 0;
};