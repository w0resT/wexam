#include <stdexcept>
#include "test.h"
#include "question_with_options.h"
#include "question_with_free_answer.h"

inline void Test::SetId( const unsigned int id ) { 
	m_id = id; 
}

inline unsigned int Test::GetId() const { 
	return m_id; 
}

inline std::string Test::GetTitle() const { 
	return m_title; 
}

inline std::string Test::GetDescription() const { 
	return m_description; 
}

inline void Test::SetTitle( const std::string& title ) { 
	m_title = title; 
}

inline void Test::SetDescription( const std::string& description ) { 
	m_description = description; 
}

inline void Test::Clear() {
	m_id = 0;
	m_title.clear();
	m_description.clear();
	m_questions.clear();
}

inline void Test::AddQuestion( unsigned int id, const std::string& question, const std::vector<std::string>& answerOptions, const std::string& correctAnswer ) {
	auto newQuestion = std::make_shared<QuestionWithAnswerOptions>( id, question, answerOptions );
	newQuestion->SetCorrectAnswer( correctAnswer );
	m_questions.emplace_back( newQuestion );
}

inline void Test::AddQuestion( unsigned int id, const std::string& question, const std::string& correctAnswer ) {
	auto newQuestion = std::make_shared<QuestionWithFreeAnswer>( id, question );
	newQuestion->SetCorrectAnswer( correctAnswer );
	m_questions.emplace_back( newQuestion );
}

inline void Test::AddQuestion( std::shared_ptr<IQuestion> question ) {
	m_questions.emplace_back( std::move( question ) );
}

inline std::shared_ptr<IQuestion> Test::GetQuestion( unsigned int idx ) const {
	if ( idx < m_questions.size() )
		return m_questions[ idx ];
	throw std::out_of_range( "Invalid question index" );
}

inline std::vector<std::shared_ptr<IQuestion>> Test::GetQuestions() const {
	return m_questions;
}

inline std::shared_ptr<IQuestion> Test::FindQuestionById( unsigned int id ) const {
	for ( const auto& question : m_questions ) {
		if ( question->GetId() == id ) {
			return question;
		}
	}
	return nullptr; // Test not found
}

inline void Test::DeleteQuestion( unsigned int idx ) {
	if ( idx < m_questions.size() )
		m_questions.erase( m_questions.begin() + idx );
	else
		throw std::out_of_range( "Invalid question index" );
}

inline void Test::EditQuestion( unsigned int idx, const std::string& question ) {
	if ( idx < m_questions.size() )
		m_questions[ idx ]->SetQuestion( question );
	else
		throw std::out_of_range( "Invalid question index" );
}

inline void Test::EditQuestion( unsigned int idx, const std::string& question, const std::vector<std::string>& answerOptions ) {
	if ( idx < m_questions.size() ) {
		m_questions[ idx ]->SetQuestion( question );
		m_questions[ idx ]->SetAnswerOptions( answerOptions );
	}
	else {
		throw std::out_of_range( "Invalid question index" );
	}
}

inline unsigned int Test::GetQuestionCount() const {
	return m_questions.size();
}
