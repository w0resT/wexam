#include "testresult.h"

void TestResult::EvaluateTest() {
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

unsigned int TestResult::GetTestId() const { 
	return m_testId; 
}

unsigned int TestResult::GetTotalQuestions() const { 
	return m_totalQuestions; 
}

unsigned int TestResult::GetCorrectAnswers() const { 
	return m_correctAnswers; 
}

unsigned int TestResult::GetIncorrectAnswers() const { 
	return m_incorrectAnswers; 
}

unsigned int TestResult::GetUnansweredQuestions() const { 
	return m_unansweredQuestions; 
}

double TestResult::GetScore() const { 
	return m_score; 
}
