#pragma once
#include <memory>
#include <string>

class ITestRepository;

class ITestManager {
public:
	virtual ~ITestManager() {}

	// Import tests from a file into the repository
	virtual void ImportTests( const std::string& filename, 
							  std::shared_ptr<ITestRepository> repository, 
							  bool useDecrypt = false, 
							  bool needCorAnswers = false, 
							  bool needUserAnswers = false ) = 0;

	// Export tests from the repository to a file
	virtual void ExportTests( const std::string& filename, 
							  std::shared_ptr<ITestRepository> repository, 
							  bool useEncrypt = false, 
							  bool needCorAnswers = false, 
							  bool needUserAnswers = false ) = 0;
};