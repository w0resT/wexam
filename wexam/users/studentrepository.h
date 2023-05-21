#pragma once
#include <vector>
#include <string>
#include "../interfaces/users/istudentrepository.h"
#include "../interfaces/users/istudent.h"

class StudentRepository : public IStudentRepository {
public:
	StudentRepository() {}

	/**
	 * Adds a student to the repository.
	 *
	 * @param student The student object to add.
	 */
	void AddStudent( const std::shared_ptr<IStudent>& student ) override;

	/**
	 * Adds multiple students to the repository.
	 *
	 * @param students The vector of student objects to add.
	 */
	void AddStudents( const std::vector<std::shared_ptr<IStudent>>& students ) override;

	/**
	 * Removes a student from the repository by ID.
	 *
	 * @param id The ID of the student to remove.
	 */
	void RemoveStudent( const unsigned int id ) override;

	/**
	 * Retrieves all the students from the repository.
	 *
	 * @return A vector of shared pointers to the student objects.
	 */
	std::vector<std::shared_ptr<IStudent>> GetStudents() const override;

	/**
	 * Retrieves a student from the repository by index.
	 *
	 * @param index The index of the student to retrieve.
	 * @return The shared pointer to the student object.
	 */
	std::shared_ptr<IStudent> GetStudent( const unsigned int index ) const override;

	/**
	 * Finds a student in the repository by ID.
	 *
	 * @param id The ID of the student to find.
	 * @return The shared pointer to the found student object, or nullptr if not found.
	 */
	std::shared_ptr<IStudent> FindStudentById( const unsigned int id ) const override;

	/**
	 * Clears the student repository, removing all students.
	 */
	void Clear() override;

private:
	std::vector<std::shared_ptr<IStudent>> m_students;
};