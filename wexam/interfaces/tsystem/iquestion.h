#pragma once
#include <string>
#include <vector>

/**
 *
 * An enumeration representing the types of questions.
 */
enum class QuestionType {
	Default,		// Default question type
	FreeAnswer,		// Question with a free answer
	AnswerOptions,	// Question with answer options
	MAX				// Maximum value for iteration purposes
};

class IQuestion {
public:
	virtual ~IQuestion() {}

	// Set the ID of the question
	virtual void SetId(unsigned int id) = 0;

	// Get the ID of the question
	virtual unsigned int GetId() const = 0;

	// Get the question text
	virtual std::string GetQuestion() const = 0;

	// Set the question text
	virtual void SetQuestion(const std::string& question) = 0;

	// Set the answer options for the question
	virtual void SetAnswerOptions(const std::vector<std::string>& answerOptions) = 0;

	// Add an answer option to the question
	virtual void AddAnswerOption(const std::string& answerOption) = 0;

	// Get the answer option at the specified index
	virtual std::string GetAnswerOption(unsigned int idx) const = 0;

	// Get all the answer options for the question
	virtual std::vector<std::string> GetAnswerOptions() const = 0;

	// Get the question type
	virtual QuestionType GetQuestionType() const = 0;

	// Set the correct answer for the question
	virtual void SetCorrectAnswer(const std::string& correctAnswer) = 0;

	// Get the correct answer for the question
	virtual std::string GetCorrectAnswer() const = 0;

	// Set the user's answer for the question
	virtual void SetUserAnswer(const std::string& userAnswer) = 0;

	// Get the user's answer for the question
	virtual std::string GetUserAnswer() const = 0;

	// Check if the question has a correct answer
	virtual bool HasCorrectAnswer() const = 0;

	// Evaluate if the answer is correct
	virtual bool IsAnswerCorrect() const = 0;
};