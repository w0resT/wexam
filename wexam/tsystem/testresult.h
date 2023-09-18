#pragma once
#include <memory>
#include "../interfaces/tsystem/itestresult.h"
#include "../interfaces/tsystem/itest.h"
#include "../interfaces/tsystem/iquestion.h"

/**
 * TestResult represents the result of a test evaluation.
 * It implements the ITestResult interface.
 */
class TestResult : public ITestResult {
public:
	/**
	 * Constructs a TestResult object with the given test.
	 *
	 * @param test A shared pointer to the test associated with the result.
	 */
	TestResult( std::shared_ptr<ITest> test )
		: m_test( std::move( test ) ),
		m_correctAnswers( 0 ),
		m_incorrectAnswers( 0 ),
		m_unansweredQuestions( 0 ),
		m_score( 0.0 ) {
		m_totalQuestions = m_test->GetQuestionCount();
		m_testId = m_test->GetId();
	}

	/**
	 * Evaluates the test and calculates the result.
	 * It counts the number of correct, incorrect, and unanswered questions.
	 * It also calculates the test score as a percentage.
	 */
	void EvaluateTest() override;

	/**
	 * Gets the ID of the test associated with the result.
	 *
	 * @return The ID of the test.
	 */
	unsigned int GetTestId() const override;

	/**
	 * Gets the total number of questions in the test.
	 *
	 * @return The total number of questions.
	 */
	unsigned int GetTotalQuestions() const override;

	/**
	 * Gets the number of correct answers in the test.
	 *
	 * @return The number of correct answers.
	 */
	unsigned int GetCorrectAnswers() const override;

	/**
	 * Gets the number of incorrect answers in the test.
	 *
	 * @return The number of incorrect answers.
	 */
	unsigned int GetIncorrectAnswers() const override;

	/**
	 * Gets the number of unanswered questions in the test.
	 *
	 * @return The number of unanswered questions.
	 */
	unsigned int GetUnansweredQuestions() const override;

	/**
	 * Gets the score of the test as a percentage.
	 *
	 * @return The test score as a percentage.
	 */
	double GetScore() const override;

	void SaveResult(const std::string student, const std::string group) override;

private:
	unsigned int m_testId;                // The ID of the test
	unsigned int m_totalQuestions;        // The total number of questions in the test
	unsigned int m_correctAnswers;        // The number of correct answers
	unsigned int m_incorrectAnswers;      // The number of incorrect answers
	unsigned int m_unansweredQuestions;   // The number of unanswered questions
	double m_score;                       // The score of the test as a percentage

	std::shared_ptr<ITest> m_test;        // A shared pointer to the associated test
};