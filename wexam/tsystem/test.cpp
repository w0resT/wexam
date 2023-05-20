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
	if ( idx < m_questions.size() )
		return m_questions[ idx ];
	throw std::out_of_range( "Invalid question index" );
}

std::vector<std::shared_ptr<IQuestion>> Test::GetQuestions() const {
	return m_questions;
}

std::shared_ptr<IQuestion> Test::FindQuestionById( unsigned int id ) const {
	for ( const auto& question : m_questions ) {
		if ( question->GetId() == id ) {
			return question;
		}
	}
	return nullptr; // Test not found
}

void Test::RemoveQuestion( unsigned int id ) {
	for ( auto it = m_questions.begin(); it != m_questions.end(); ++it ) {
		if ( ( *it )->GetId() == id ) {
			m_questions.erase( it );
			return;
		}
	}
	throw std::invalid_argument( "Question not found" );
}

void Test::EditQuestion( unsigned int idx, const std::string& question ) {
	if ( idx < m_questions.size() )
		m_questions[ idx ]->SetQuestion( question );
	else
		throw std::out_of_range( "Invalid question index" );
}

void Test::EditQuestion( unsigned int idx, const std::string& question, const std::vector<std::string>& answerOptions ) {
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
