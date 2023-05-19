#pragma once
#include "../interfaces/tsystem/iquestion.h"

class QuestionWithAnswerOptions : public IQuestion {
public:
	/**
	 * Default constructor for QuestionWithAnswerOptions.
	 * Initializes the ID to 0.
	 */
	QuestionWithAnswerOptions()
		: m_id( 0 ) {}

	/**
	 * Constructor for QuestionWithAnswerOptions.
	 *
	 * @param id The ID of the question.
	 * @param question The text of the question.
	 * @param answerOptions The answer options for the question.
	 */
	QuestionWithAnswerOptions( unsigned int id, const std::string& question, const std::vector<std::string>& answerOptions )
		: m_id( id ),
		m_correctAnswer( "" ),
		m_question( question ),
		m_answerOptions( answerOptions ) {}

	/**
	 * Set the ID of the question.
	 *
	 * @param id The ID to be set.
	 */
	void SetId( unsigned int id ) override;

	/**
	 * Get the ID of the question.
	 *
	 * @return The ID of the question.
	 */
	unsigned int GetId() const override;

	/**
	 * Get the text of the question.
	 *
	 * @return The text of the question.
	 */
	std::string GetQuestion() const override;

	/**
	 * Set the text of the question.
	 *
	 * @param question The text of the question.
	 */
	void SetQuestion( const std::string& question ) override;

	/**
	 * Set the answer options for the question.
	 *
	 * @param answerOptions The answer options to be set.
	 */
	void SetAnswerOptions( const std::vector<std::string>& answerOptions ) override;

	/**
	 * Add an answer option to the question.
	 *
	 * @param answerOption The answer option to be added.
	 */
	void AddAnswerOption( const std::string& answerOption ) override;

	/**
	 * Get an answer option by its index.
	 *
	 * @param idx The index of the answer option.
	 * @return The answer option.
	 * @throws std::out_of_range If the index is invalid.
	 */
	std::string GetAnswerOption( unsigned int idx ) const override;

	/**
	 * Get all the answer options of the question.
	 *
	 * @return A vector of answer options.
	 */
	std::vector<std::string> GetAnswerOptions() const override;

	/**
	 * Get the type of the question.
	 *
	 * @return The question type.
	 */
	QuestionType GetQuestionType() const override;

	/**
	 * Set the correct answer for the question.
	 *
	 * @param correctAnswer The correct answer.
	 */
	void SetCorrectAnswer( const std::string& correctAnswer ) override;

	/**
	 * Get the correct answer for the question.
	 *
	 * @return The correct answer.
	 */
	std::string GetCorrectAnswer() const override;

	/**
	 * Set the user's answer for the question.
	 *
	 * @param userAnswer The user's answer.
	 */
	void SetUserAnswer( const std::string& userAnswer ) override;

	/**
	 * Get the user's answer for the question.
	 *
	 *@return The user's answer.
	 */
	std::string GetUserAnswer() const override;

	/**
	 *
	 * Check if the question has a correct answer defined.
	 * @return True if the question has a correct answer, false otherwise.
	 */
	bool HasCorrectAnswer() const override;

	/**
	 *
	 * Check if the user's answer is correct.
	 * @return True if the user's answer is correct, false otherwise.
	 */
	bool IsAnswerCorrect() const override;

private:
	unsigned int m_id;							// The ID of the question
	std::string m_question;						// The text of the question
	std::string m_userAnswer;					// The user's answer to the question
	std::string m_correctAnswer;				// The correct answer to the question
	std::vector<std::string> m_answerOptions;	// The answer options for the question
};