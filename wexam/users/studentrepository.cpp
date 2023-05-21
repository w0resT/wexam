#include <stdexcept>
#include "studentrepository.h"

void StudentRepository::AddStudent( const std::shared_ptr<IStudent>& student ) {
	m_students.emplace_back( student );
}

void StudentRepository::AddStudents( const std::vector<std::shared_ptr<IStudent>>& students ) {
	m_students = students;
}

void StudentRepository::RemoveStudent( const unsigned int id ) {
	m_students.erase( std::remove_if( m_students.begin(), m_students.end(),
									  [ & ] ( const std::shared_ptr<IStudent>& student ) { return student->GetId() == id; } ),
					  m_students.end() );
}

std::vector<std::shared_ptr<IStudent>> StudentRepository::GetStudents() const {
	return m_students;
}

std::shared_ptr<IStudent> StudentRepository::GetStudent( const unsigned int index ) const {
	if ( index < m_students.size() ) {
		return m_students[ index ];
	}
	throw std::out_of_range( "Invalid student index" );
}

std::shared_ptr<IStudent> StudentRepository::FindStudentById( const unsigned int id ) const {
	for ( const auto& student : m_students ) {
		if ( student->GetId() == id ) {
			return student;
		}
	}
	return nullptr; // Student not found
}

void StudentRepository::Clear() {
	m_students.clear();
}
