#pragma once
#include <vector>
#include <string>
#include <memory>

class IStudent;

class IStudentRepository {
public:
	virtual ~IStudentRepository() {}

	// Adds a student to the repository.
	virtual void AddStudent( const std::shared_ptr<IStudent>& student ) = 0;

	// Adds multiple students to the repository.
	virtual void AddStudents( const std::vector<std::shared_ptr<IStudent>>& students ) = 0;

	// Removes a student from the repository by their ID.
	virtual void RemoveStudent( const unsigned int id ) = 0;

	// Returns all the students in the repository.
	virtual std::vector<std::shared_ptr<IStudent>> GetStudents() const = 0;

	// Returns a student from the repository by their index.
	virtual std::shared_ptr<IStudent> GetStudent( const unsigned int index ) const = 0;

	// Finds a student in the repository by their ID.
	virtual std::shared_ptr<IStudent> FindStudentById( const unsigned int id ) const = 0;

	// Finds a student in the repository by their name.
	virtual std::shared_ptr<IStudent> FindStudentByName( const std::string& name ) const = 0;

	// Clears the repository, removing all students.
	virtual void Clear() = 0;
};