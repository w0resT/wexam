#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "testresult.h"
#include <sstream>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>

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

void TestResult::SaveResult(const std::string student, const std::string group) {
    // Get the current local time as a string
    auto currentTime = std::chrono::system_clock::now();
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    std::tm* localTime = std::localtime(&currentTime_t);
    std::ostringstream dateTimeStream;
    dateTimeStream << std::put_time(localTime, "%Y-%m-%d_%H-%M-%S");
    std::string dateTimeString = dateTimeStream.str();

    // Construct the filename
    std::string filename = "test_" + dateTimeString + ".txt";

    // Open the file for writing
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    // Compose the result string
    std::ostringstream resultStream;
    resultStream << "[Name: " << student << ", group: " << group << "]\n";
    resultStream << "[Test: " << m_test->GetTitle() << "]\n";
    resultStream << "Total questions: " << m_totalQuestions << "\n";
    resultStream << "Correct: " << m_correctAnswers << "\n";
    resultStream << "Incorrect: " << m_incorrectAnswers << "\n";
    resultStream << "Unanswered: " << m_unansweredQuestions << "\n";
    resultStream << "Score: " << m_score << "%; Approximate test score: ";
    resultStream << (static_cast<int>(m_score) > 80 ? "5" :
                        static_cast<int>(m_score) > 60 ? "4" :
                        static_cast<int>(m_score) > 40 ? "3" : "2");

    // Write the result string to the file
    file << resultStream.str();

    // Close the file
    file.close();
}
