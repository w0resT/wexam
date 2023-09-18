#include <stdexcept>
#include "test.h"
#include "question_with_options.h"
#include "question_with_free_answer.h"

void Test::SetId( const unsigned int id ) { 
	m_id = id; 
}

unsigned int Test::GetId() const { 
	return m_id; 
}

std::string Test::GetTitle() const { 
	return m_title; 
}

std::string Test::GetDescription() const { 
	return m_description; 
}

void Test::SetTitle( const std::string& title ) { 
	m_title = title; 
}

void Test::SetDescription( const std::string& description ) { 
	m_description = description; 
}

void Test::Clear() {
	m_id = 0;
	m_title.clear();
	m_description.clear();
	m_questions.clear();
}

void Test::AddQuestion( unsigned int id, const std::string& question, const std::vector<std::string>& answerOptions, const std::string& correctAnswer ) {
	auto newQuestion = std::make_shared<QuestionWithAnswerOptions>( id, question, answerOptions );
	newQuestion->SetCorrectAnswer( correctAnswer );
	m_questions.emplace_back( newQuestion );
}

void Test::AddQuestion( unsigned int id, const std::string& question, const std::string& correctAnswer ) {
	auto newQuestion = std::make_shared<QuestionWithFreeAnswer>( id, question );
	newQuestion->SetCorrectAnswer( correctAnswer );
	m_questions.emplace_back( newQuestion );
}

void Test::AddQuestion( std::shared_ptr<IQuestion> question ) {
	m_questions.emplace_back( std::move( question ) );
}

std::shared_ptr<IQuestion> Test::GetQuestion( unsigned int idx ) const {
	if ( m_questions.empty() ) {
		throw std::logic_error( "No questions in the test" );
	}

	if ( idx < m_questions.size() )
		return m_questions[ idx ];
	throw std::out_of_range( "Invalid question index" );
}

std::vector<std::shared_ptr<IQuestion>> Test::GetQuestions() const {
	return m_questions;
}

std::shared_ptr<IQuestion> Test::FindQuestionById( unsigned int id ) const {
	if ( m_questions.empty() ) {
		throw std::logic_error( "No questions in the test" );
	}

	auto it = std::find_if(m_questions.begin(), m_questions.end(),
							[&](const std::shared_ptr<IQuestion>& question) {
								if (question) return question->GetId() == id; });

	if ( it != m_questions.end() ) {
		return *it;
	}

	return nullptr; // Test not found
}

void Test::RemoveQuestion( unsigned int id ) {
	if ( m_questions.empty() ) {
		throw std::logic_error( "No questions in the test" );
	}

	m_questions.erase( std::remove_if( m_questions.begin(), m_questions.end(),
									  [ & ] ( const std::shared_ptr<IQuestion>& question ) { return question->GetId() == id; } ),
					   m_questions.end() );
}

void Test::EditQuestion( unsigned int idx, const std::string& question ) {
	if ( m_questions.empty() ) {
		throw std::logic_error( "No questions in the test" );
	}

	if ( idx < m_questions.size() )
		m_questions[ idx ]->SetQuestion( question );
	else
		throw std::out_of_range( "Invalid question index" );
}

void Test::EditQuestion( unsigned int idx, const std::string& question, const std::vector<std::string>& answerOptions ) {
	if ( m_questions.empty() ) {
		throw std::logic_error( "No questions in the test" );
	}

	if ( idx < m_questions.size() ) {
		m_questions[ idx ]->SetQuestion( question );
		m_questions[ idx ]->SetAnswerOptions( answerOptions );
	}
	else {
		throw std::out_of_range( "Invalid question index" );
	}
}

unsigned int Test::GetQuestionCount() const {
	return m_questions.size();
}
