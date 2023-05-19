#pragma once

/**
 * ITestResult is an interface that represents the result of a test evaluation.
 * It provides methods to evaluate the test, retrieve test statistics, and calculate the test score.
 */
class ITestResult {
public:
	virtual ~ITestResult() {}

	// Evaluate the test and calculate the result
	virtual void EvaluateTest() = 0;

	// Get the ID of the test
	virtual unsigned int GetTestId() const = 0;

	// Get the total number of questions in the test
	virtual unsigned int GetTotalQuestions() const = 0;

	// Get the number of correct answers
	virtual unsigned int GetCorrectAnswers() const = 0;

	// Get the number of incorrect answers
	virtual unsigned int GetIncorrectAnswers() const = 0;

	// Get the number of unanswered questions
	virtual unsigned int GetUnansweredQuestions() const = 0;

	// Get the score of the test as a percentage
	virtual double GetScore() const = 0;
};