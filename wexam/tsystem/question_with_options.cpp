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

inline void QuestionWithAnswerOptions::SetQuestion( const std::string& question ) {
	m_question = question;
}

inline void QuestionWithAnswerOptions::SetAnswerOptions( const std::vector<std::string>& answerOptions ) {
	m_answerOptions = answerOptions;
}

inline void QuestionWithAnswerOptions::AddAnswerOption( const std::string& answerOption ) {
	m_answerOptions.push_back( answerOption );
}

inline std::string QuestionWithAnswerOptions::GetAnswerOption( unsigned int idx ) const {
	if ( idx < m_answerOptions.size() )
		return m_answerOptions[ idx ];
	throw std::out_of_range( "Invalid answer option ID" );
}

inline std::vector<std::string> QuestionWithAnswerOptions::GetAnswerOptions() const {
	return m_answerOptions;
}

inline QuestionType QuestionWithAnswerOptions::GetQuestionType() const {
	return QuestionType::AnswerOptions;
}

inline void QuestionWithAnswerOptions::SetCorrectAnswer( const std::string& correctAnswer ) {
	m_correctAnswer = correctAnswer;
}

inline std::string QuestionWithAnswerOptions::GetCorrectAnswer() const {
	return m_correctAnswer;
}

inline void QuestionWithAnswerOptions::SetUserAnswer( const std::string& userAnswer ) {
	m_userAnswer = userAnswer;
}

inline std::string QuestionWithAnswerOptions::GetUserAnswer() const {
	return m_userAnswer;
}

inline bool QuestionWithAnswerOptions::HasCorrectAnswer() const {
	return !m_correctAnswer.empty();
}

inline bool QuestionWithAnswerOptions::IsAnswerCorrect() const {
	return m_userAnswer == m_correctAnswer;
}
