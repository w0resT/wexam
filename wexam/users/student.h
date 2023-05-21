#pragma once
#include <string>
#include "../interfaces/users/istudent.h"

class Student : public IStudent {
public:
	/**
	 * Default constructor for Student.
	 * Initializes the ID to 0.
	 */
	Student() 
		: m_id(0) {}

	/**
	 * Constructor for Student.
	 *
	 * @param id The ID of the student.
	 * @param name The name of the student.
	 * @param group The group of the student.
	 */
	Student( const unsigned int id, const std::string& name, const std::string& group )
		: m_id( id ), m_name( name ), m_group( group ) {}

	/**
	 * Get the ID of the student.
	 *
	 * @return The ID of the student.
	 */
	unsigned int GetId() const override;

	/**
	 * Set the ID of the student.
	 *
	 * @param id The ID of the student.
	 */
	void SetId( const unsigned int id ) override;

	/**
	 * Get the name of the student.
	 *
	 * @return The name of the student.
	 */
	std::string GetName() const override;

	/**
	 * Set the name of the student.
	 *
	 * @param name The name of the student.
	 */
	void SetName( const std::string& name ) override;

	/**
	 * Get the group of the student.
	 *
	 * @return The group of the student.
	 */
	std::string GetGroup() const override;

	/**
	 * Set the group of the student.
	 *
	 * @param group The group of the student.
	 */
	void SetGroup( const std::string& group ) override;

private:
	unsigned int m_id;       // The ID of the student
	std::string m_name;      // The name of the student
	std::string m_group;     // The group of the student
};