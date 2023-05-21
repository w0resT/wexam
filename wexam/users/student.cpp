#include "student.h"

unsigned int Student::GetId() const {
	return m_id;
}

void Student::SetId( const unsigned int id ) {
	m_id = id;
}

std::string Student::GetName() const {
	return m_name;
}

void Student::SetName( const std::string& name ) {
	m_name = name;
}

std::string Student::GetGroup() const {
	return m_group;
}

void Student::SetGroup( const std::string& group ) {
	m_group = group;
}
