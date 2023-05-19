#pragma once
#include "../interfaces/tsystem/itestrepository.h"
#include "../interfaces/tsystem/itest.h"

/**
 * @class TestRepository
 * @brief Implementation of the ITestRepository interface.
 */
class TestRepository : public ITestRepository {
public:
	TestRepository() {}

	/**
	 * Add a new test to the repository.
	 *
	 * @param test The test to be added.
	 */
	void AddTest( const std::shared_ptr<ITest>& test ) override;

	/**
	 * Add a new test to the repository.
	 *
	 * @param tests The tests to be added (vector).
	 */
	void AddTests( const std::vector<std::shared_ptr<ITest>>& tests ) override;

	/**
	 * Get the number of tests in the repository.
	 *
	 * @return The number of tests in the repository.
	 */
	unsigned int GetTestCount() const override;

	/**
	 * Get a test by its index.
	 *
	 * @param index The index of the test.
	 * @return A shared pointer to the test.
	 * @throws std::out_of_range if the index is invalid.
	 */
	std::shared_ptr<ITest> GetTest( unsigned int index ) const override;

	/**
	 * Get all the tests stored in the test repository.
	 *
	 * @return A vector containing shared pointers to the tests.
	 */
	std::vector<std::shared_ptr<ITest>> GetTests() const override;

	/**
	 * Find a test by its ID.
	 *
	 * @param id The ID of the test.
	 * @return A shared pointer to the test if found, nullptr otherwise.
	 */
	std::shared_ptr<ITest> FindTestById( unsigned int id ) const override;

	/**
	 * Remove a test from the repository by its ID.
	 *
	 * @param id The ID of the test to be removed.
	 * @throws std::invalid_argument if the test is not found.
	 */
	void RemoveTest( unsigned int id ) override;

	/**
	 * Modify a test in the repository.
	 *
	 * @param test The modified test.
	 */
	void ModifyTest( const std::shared_ptr<ITest>& test ) override;

private:
	std::vector<std::shared_ptr<ITest>> m_tests;	// A container storing shared pointers to Test objects
};