#pragma once
#include "../interfaces/tsystem/itest.h"
#include "../interfaces/tsystem/iquestion.h"

class Test : public ITest {
public:
	/**
	 * Default constructor. Initializes the ID to 0.
	 */
	Test()
		: m_id( 0 ) {}

	/**
	 * Copy constructor. Creates a new test with the same properties as the given test.
	 *
	 * @param test The test to be copied.
	 */
	Test( const std::shared_ptr<ITest>& test )
		: m_id( test->GetId() ), m_title( test->GetTitle() ), m_description( test->GetDescription() ), m_questions( test->GetQuestions() ) {}

	/**
	 * Constructor with parameters. Creates a new test with the specified ID, title, and description.
	 *
	 * @param id The ID of the test.
	 * @param title The title of the test.
	 * @param description The description of the test.
	 */
	Test( const unsigned int id, const std::string& title, const std::string& description )
		: m_id( id ), m_title( title ), m_description( description ) {}

	/**
	 * Set the ID of the test.
	 *
	 * @param id The ID to be set.
	 */
	void SetId( const unsigned int id ) override;

	/**
	 * Get the ID of the test.
	 *
	 * @return The ID of the test.
	 */
	unsigned int GetId() const override;

	/**
	 * Get the title of the test.
	 *
	 * @return The title of the test.
	 */
	std::string GetTitle() const override;

	/**
	 * Get the description of the test.
	 *
	 * @return The description of the test.
	 */
	std::string GetDescription() const override;

	/**
	 * Set the title of the test.
	 *
	 * @param title The title to be set.
	 */
	void SetTitle( const std::string& title ) override;

	/**
	 * Set the description of the test.
	 *
	 * @param description The description to be set.
	 */
	void SetDescription( const std::string& description ) override;

	/**
	 * Clear the test by resetting all its properties and removing all questions.
	 */
	void Clear() override;

	/**
	 * Add a question with answer options to the test.
	 *
	 * @param id The ID of the question.
	 * @param question The question text.
	 * @param answerOptions The answer options for the question.
	 * @param correctAnswer The correct answer for the question (optional).
	 */
	void AddQuestion( unsigned int id, const std::string& question, const std::vector<std::string>& answerOptions, const std::string& correctAnswer = "" ) override;

	/**
	 * Add a question with free answer to the test.
	 *
	 * @param id The ID of the question.
	 * @param question The question text.
	 * @param correctAnswer The correct answer for the question (optional).
	 */
	void AddQuestion( unsigned int id, const std::string& question, const std::string& correctAnswer = "" ) override;

	/**
	 * Add a question to the test.
	 *
	 * @param question The question to be added.
	 */
	void AddQuestion( std::shared_ptr<IQuestion> question ) override;

	/**
	 * Get a question from the test by its index.
	 *
	 * @param idx The index of the question.
	 * @return A shared pointer to the question.
	 * @throws std::out_of_range if the index is invalid.
	 */
	std::shared_ptr<IQuestion> GetQuestion( unsigned int idx ) const override;

	/**
	 * Get all the questions in the test.
	 *
	 * @return A vector containing shared pointers to the questions.
	 */
	std::vector<std::shared_ptr<IQuestion>> GetQuestions() const override;

	/**
	 * Find a question in the test by its ID.
	 *
	 * @param id The ID of the question to be found.
	 * @return A shared pointer to the question if found, nullptr otherwise.
	 */
	std::shared_ptr<IQuestion> FindQuestionById( unsigned int id ) const override;

	/**
	 * Delete a question from the test by its index.
	 *
	 * @param idx The index of the question to be deleted.
	 * @throws std::out_of_range if the index is invalid.
	 */
	void RemoveQuestion( unsigned int idx ) override;

	/**
	 * Edit the text of a question in the test by its index.
	 *
	 * @param idx The index of the question to be edited.
	 * @param question The new question text.
	 * @throws std::out_of_range if the index is invalid.
	 */
	void EditQuestion( unsigned int idx, const std::string& question ) override;

	/**
	 * Edit the text and answer options of a question in the test by its index.
	 *
	 * @param idx The index of the question to be edited.
	 * @param question The new question text.
	 * @param answerOptions The new answer options for the question.
	 * @throws std::out_of_range if the index is invalid.
	 */
	void EditQuestion( unsigned int idx, const std::string& question, const std::vector<std::string>& answerOptions ) override;

	/**
	 * Get the total number of questions in the test.
	 *
	 * @return The number of questions.
	 */
	unsigned int GetQuestionCount() const override;

private:
	unsigned int m_id;			// The ID of the test
	std::string m_title;		// The title of the test
	std::string m_description;	// The description of the test

	std::vector<std::shared_ptr<IQuestion>> m_questions; // The questions in the test

};