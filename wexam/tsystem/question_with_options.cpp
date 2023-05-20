#include <stdexcept>
#include "question_with_options.h"

void QuestionWithAnswerOptions::SetId( unsigned int id ) {
	m_id = id;
}

unsigned int QuestionWithAnswerOptions::GetId() const {
	return m_id;
}

std::string QuestionWithAnswerOptions::GetQuestion() const {
	return m_question;
}

void QuestionWithAnswerOptions::SetQuestion( const std::string& question ) {
	m_question = question;
}

void QuestionWithAnswerOptions::SetAnswerOptions( const std::vector<std::string>& answerOptions ) {
	m_answerOptions = answerOptions;
}

void QuestionWithAnswerOptions::AddAnswerOption( const std::string& answerOption ) {
	m_answerOptions.push_back( answerOption );
}

std::string QuestionWithAnswerOptions::GetAnswerOption( unsigned int idx ) const {
	if ( idx < m_answerOptions.size() )
		return m_answerOptions[ idx ];
	throw std::out_of_range( "Invalid answer option ID" );
}

std::vector<std::string> QuestionWithAnswerOptions::GetAnswerOptions() const {
	return m_answerOptions;
}

QuestionType QuestionWithAnswerOptions::GetQuestionType() const {
	return QuestionType::AnswerOptions;
}

void QuestionWithAnswerOptions::SetCorrectAnswer( const std::string& correctAnswer ) {
	m_correctAnswer = correctAnswer;
}

std::string QuestionWithAnswerOptions::GetCorrectAnswer() const {
	return m_correctAnswer;
}

void QuestionWithAnswerOptions::SetUserAnswer( const std::string& userAnswer ) {
	m_userAnswer = userAnswer;
}

std::string QuestionWithAnswerOptions::GetUserAnswer() const {
	return m_userAnswer;
}

bool QuestionWithAnswerOptions::HasCorrectAnswer() const {
	return !m_correctAnswer.empty();
}

bool QuestionWithAnswerOptions::IsAnswerCorrect() const {
	return m_userAnswer == m_correctAnswer;
}
