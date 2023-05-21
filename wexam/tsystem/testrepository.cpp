#include <stdexcept>
#include <algorithm>
#include "testrepository.h"

void TestRepository::AddTest( const std::shared_ptr<ITest>& test ) {
	m_tests.emplace_back( test );
}

void TestRepository::AddTests( const std::vector<std::shared_ptr<ITest>>& tests ) {
	for ( const auto& test : tests ) {
		m_tests.emplace_back( test );
	}
}

unsigned int TestRepository::GetTestCount() const {
	return m_tests.size();
}

std::shared_ptr<ITest> TestRepository::GetTest( unsigned int index ) const {
	if ( index < m_tests.size() ) {
		return m_tests[ index ];
	}
	throw std::out_of_range( "Invalid test index" );
}

std::vector<std::shared_ptr<ITest>> TestRepository::GetTests() const {
	return m_tests;
}

std::shared_ptr<ITest> TestRepository::FindTestById( unsigned int id ) const {
	for ( const auto& test : m_tests ) {
		if ( test->GetId() == id ) {
			return test;
		}
	}
	return nullptr; // Test not found
}

void TestRepository::RemoveTest( unsigned int id ) {
	for ( auto it = m_tests.begin(); it != m_tests.end(); ++it ) {
		if ( ( *it )->GetId() == id ) {
			m_tests.erase( it );
			return;
		}
	}
	throw std::invalid_argument( "Test not found" );
}

void TestRepository::ModifyTest( const std::shared_ptr<ITest>& test ) {
	auto it = std::find_if( m_tests.begin(), m_tests.end(), 
							[ test ] ( const std::shared_ptr<ITest>& storedTest ) 
							{ return storedTest->GetId() == test->GetId(); } );

	if ( it != m_tests.end() ) {
		*it = test;
	}
}

void TestRepository::Clear() {
	m_tests.clear();
}
