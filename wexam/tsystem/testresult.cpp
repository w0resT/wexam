#include "testresult.h"

inline void TestResult::EvaluateTest() {
	for ( const auto& question : m_test->GetQuestions() ) {
		if ( question->GetUserAnswer() == question->GetCorrectAnswer() ) {
			m_correctAnswers++;
		}
		else if ( question->GetUserAnswer().empty() ) {
			m_unansweredQuestions++;
		}
		else {
			m_incorrectAnswers++;
		}
	}

	// Calculate the score based on the number of correct answers
	m_score = static_cast< float >( m_correctAnswers ) / m_totalQuestions * 100.0;
}

inline unsigned int TestResult::GetTestId() const { 
	return m_testId; 
}

inline unsigned int TestResult::GetTotalQuestions() const { 
	return m_totalQuestions; 
}

inline unsigned int TestResult::GetCorrectAnswers() const { 
	return m_correctAnswers; 
}

inline unsigned int TestResult::GetIncorrectAnswers() const { 
	return m_incorrectAnswers; 
}

inline unsigned int TestResult::GetUnansweredQuestions() const { 
	return m_unansweredQuestions; 
}

inline double TestResult::GetScore() const { 
	return m_score; 
}
