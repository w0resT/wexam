#pragma once
#include <vector>
#include <memory>

class ITest;

/**
 * @class ITestRepository
 * @brief Interface for a test repository.
 */
class ITestRepository {
public:
	virtual ~ITestRepository() {}

	// Add a test to the repository
	virtual void AddTest(const std::shared_ptr<ITest>& test) = 0;

	// Add a tests to the repository
	virtual void AddTests(const std::vector<std::shared_ptr<ITest>>& tests) = 0;

	// Remove a test from the repository by its ID
	virtual void RemoveTest(unsigned int testId) = 0;

	// Get a test from the repository by its ID
	virtual std::shared_ptr<ITest> GetTest(unsigned int testId) const = 0;

	// Get all tests from the repository
	virtual std::vector<std::shared_ptr<ITest>> GetTests() const = 0;

	// Find a test by its ID
	virtual std::shared_ptr<ITest> FindTestById(unsigned int testId) const = 0;

	// Modify a test in the repository
	virtual void ModifyTest(const std::shared_ptr<ITest>& test) = 0;

	// Get the number of tests in the repository
	virtual unsigned int GetTestCount() const = 0;
};