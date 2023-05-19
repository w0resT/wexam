#include <stdexcept>
#include "question_with_free_answer.h"

void QuestionWithFreeAnswer::SetId(unsigned int id) {
	m_id = id;
}

unsigned int QuestionWithFreeAnswer::GetId() const {
	return m_id;
}

std::string QuestionWithFreeAnswer::GetQuestion() const {
	return m_question;
}

void QuestionWithFreeAnswer::SetQuestion( const std::string& question ) {
	m_question = question;
}

void QuestionWithFreeAnswer::SetAnswerOptions( const std::vector<std::string>& answerOptions ) {
	throw std::logic_error( "Question with free answer does not support answer options" );
}

void QuestionWithFreeAnswer::AddAnswerOption( const std::string& answerOption ) {
	throw std::logic_error( "Question with free answer does not support answer options" );
}

std::string QuestionWithFreeAnswer::GetAnswerOption( unsigned int id ) const {
	throw std::logic_error( "Question with free answer does not support answer options" );
}

std::vector<std::string> QuestionWithFreeAnswer::GetAnswerOptions() const {
	throw std::logic_error( "Question with free answer does not support answer options" );
}

QuestionType QuestionWithFreeAnswer::GetQuestionType() const {
	return QuestionType::FreeAnswer;
}

void QuestionWithFreeAnswer::SetCorrectAnswer( const std::string& correctAnswer ) {
	m_correctAnswer = correctAnswer;
}

std::string QuestionWithFreeAnswer::GetCorrectAnswer() const {
	return m_correctAnswer;
}

void QuestionWithFreeAnswer::SetUserAnswer( const std::string& userAnswer ) {
	m_userAnswer = userAnswer;
}

std::string QuestionWithFreeAnswer::GetUserAnswer() const {
	return m_userAnswer;
}

bool QuestionWithFreeAnswer::HasCorrectAnswer() const {
	return !m_correctAnswer.empty();
}

bool QuestionWithFreeAnswer::IsAnswerCorrect() const {
	return m_userAnswer == m_correctAnswer;
}
