#pragma once
#include "../interfaces/tsystem/iquestion.h"

class QuestionWithFreeAnswer : public IQuestion {
public:
	/**
	 * Default constructor for QuestionWithFreeAnswer.
	 * Initializes the ID to 0.
	 */
	QuestionWithFreeAnswer()
		: m_id( 0 ) {}

	/**
	 * Constructor for QuestionWithFreeAnswer.
	 *
	 * @param id The ID of the question.
	 * @param question The text of the question.
	 */
	QuestionWithFreeAnswer( unsigned int id, const std::string& question )
		: m_id( id ),
		m_correctAnswer( "" ),
		m_question( question ) {}

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
	 * This function throws a logic_error as question with free answer does not support answer options.
	 *
	 * @param answerOptions The answer options to be set.
	 * @throws std::logic_error Always thrown as question with free answer does not support answer options.
	 */
	void SetAnswerOptions( const std::vector<std::string>& answerOptions ) override;

	/**
	 * Add an answer option to the question.
	 * This function throws a logic_error as question with free answer does not support answer options.
	 *
	 * @param answerOption The answer option to be added.
	 * @throws std::logic_error Always thrown as question with free answer does not support answer options.
	 */
	void AddAnswerOption( const std::string& answerOption ) override;

	/**
	 * Get an answer option by its ID.
	 * This function throws a logic_error as question with free answer does not support answer options.
	 *
	 * @param id The ID of the answer option.
	 * @return The answer option.
	 * @throws std::logic_error Always thrown as question with free answer does not support answer options.
	 */
	std::string GetAnswerOption( unsigned int id ) const override;

	/**
	 * Get all the answer options of the question.
	 * This function throws a logic_error as question with free answer does not support answer options.
	 *
	 * @return A vector of answer options.
	 * @throws std::logic_error Always thrown as question with free answer does not support answer options.
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
	 * @return The user's answer.
	 */
	std::string GetUserAnswer() const override;

	/**
	 * Check if the question has a correct answer defined.
	 *
	 * @return True if the question has a correct answer, false otherwise.
	 */
	bool HasCorrectAnswer() const override;

	/**
	 * Check if the user's answer is correct.
	 *
	 * @return True if the user's answer is correct, false otherwise.
	 */
	bool IsAnswerCorrect() const override;

private:
	unsigned int m_id;				// The ID of the question
	std::string m_question;			// The text of the question
	std::string m_userAnswer;		// The user's answer to the question
	std::string m_correctAnswer;	// The correct answer to the question
};