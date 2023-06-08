#include <stdexcept>
#include <algorithm>
#include "studentrepository.h"

void StudentRepository::AddStudent( const std::shared_ptr<IStudent>& student ) {
	if ( !student ) {
		throw std::invalid_argument( "Invalid student pointer" );
	}

	auto it = std::find_if( m_students.begin(), m_students.end(),
							[ & ] ( const std::shared_ptr<IStudent>& existingStudent ) {
								return existingStudent->GetId() == student->GetId(); } );

	if ( it != m_students.end() ) {
		throw std::runtime_error( "Duplicate student found" );
	}

	m_students.emplace_back( student );
}

void StudentRepository::AddStudents( const std::vector<std::shared_ptr<IStudent>>& students ) {
	for ( const auto& student : students ) {
		if ( !student ) {
			throw std::invalid_argument( "Invalid student pointer" );
		}
	}

	m_students = students;
}

void StudentRepository::RemoveStudent( const unsigned int id ) {
	if ( m_students.empty() ) {
		throw std::logic_error( "No students found in the repository" );
	}

	m_students.erase( std::remove_if( m_students.begin(), m_students.end(),
									  [ & ] ( const std::shared_ptr<IStudent>& student ) { return student->GetId() == id; } ),
					  m_students.end() );
}

std::vector<std::shared_ptr<IStudent>> StudentRepository::GetStudents() const {
	return m_students;
}

std::shared_ptr<IStudent> StudentRepository::GetStudent( const unsigned int index ) const {
	if ( m_students.empty() ) {
		throw std::logic_error( "No students found in the repository" );
	}

	if ( index < m_students.size() ) {
		return m_students[ index ];
	}
	throw std::out_of_range( "Invalid student index" );
}

std::shared_ptr<IStudent> StudentRepository::FindStudentById( const unsigned int id ) const {
	if ( m_students.empty() ) {
		throw std::logic_error( "No students found in the repository" );
	}

	auto it = std::find_if( m_students.begin(), m_students.end(),
							[ & ] ( const std::shared_ptr<IStudent>& student ) {
								return student->GetId() == id; } );

	if ( it != m_students.end() ) {
		return *it;
	}

	return nullptr; // Student not found
}

std::shared_ptr<IStudent> StudentRepository::FindStudentByName( const std::string& name ) const {
	if ( m_students.empty() ) {
		throw std::logic_error( "No students found in the repository" );
	}

	auto it = std::find_if( m_students.begin(), m_students.end(),
							[ & ] ( const std::shared_ptr<IStudent>& student ) {
								return student->GetName() == name; } );

	if ( it != m_students.end() ) {
		return *it;
	}

	return nullptr; // Student not found
}

void StudentRepository::Clear() {
	m_students.clear();
}
