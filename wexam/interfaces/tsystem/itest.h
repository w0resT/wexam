#pragma once
#include <string>
#include <vector>
#include <memory>

class IQuestion;

class ITest {
public:
	virtual ~ITest() {}

	// Set the ID of the test
	virtual void SetId(const unsigned int id) = 0;

	// Get the ID of the test
	virtual unsigned int GetId() const = 0;

	// Get the title of the test
	virtual std::string GetTitle() const = 0;

	// Get the description of the test
	virtual std::string GetDescription() const = 0;

	// Set the title of the test
	virtual void SetTitle(const std::string& title) = 0;

	// Set the description of the test
	virtual void SetDescription(const std::string& description) = 0;

	// Add a question to the test
	virtual void AddQuestion(unsigned int id, const std::string& question, const std::vector<std::string>& answerOptions, const std::string& correctAnswer = "") = 0;

	// Add a question to the test without answer options
	virtual void AddQuestion(unsigned int id, const std::string& question, const std::string& correctAnswer = "") = 0;

	// Add a question to the test by test
	virtual void AddQuestion(std::shared_ptr<IQuestion> question) = 0;

	// Get a question at the specified index
	virtual std::shared_ptr<IQuestion> GetQuestion(unsigned int index) const = 0;

	// Get all the questions in the test
	virtual std::vector<std::shared_ptr<IQuestion>> GetQuestions() const = 0;

	// Find a question by its ID
	virtual std::shared_ptr<IQuestion> FindQuestionById(unsigned int id) const = 0;

	// Delete a question at the specified index
	virtual void DeleteQuestion(unsigned int index) = 0;

	// Edit a question at the specified index
	virtual void EditQuestion(unsigned int index, const std::string& question) = 0;

	// Edit a question at the specified index with answer options
	virtual void EditQuestion(unsigned int index, const std::string& question, const std::vector<std::string>& answerOptions) = 0;

	// Get the number of questions in the test
	virtual unsigned int GetQuestionCount() const = 0;

	// Clear all data
	virtual void Clear() = 0;
};



