#pragma once
#include <string>

class IStudent {
public:
	virtual ~IStudent() {}

	// Returns the student's ID.
	virtual unsigned int GetId() const = 0;

	// Sets the student's ID.
	virtual void SetId( const unsigned int id ) = 0;

	// Returns the student's name.
	virtual std::string GetName() const = 0;

	// Sets the student's name.
	virtual void SetName(const std::string& name) = 0;

	// Returns the group in which the student is enrolled.
	virtual std::string GetGroup() const = 0;

	// Sets the group in which the student is enrolled.
	virtual void SetGroup( const std::string& group ) = 0;
};