#include "CppUnitTest.h"
#include "../wexam/security/totp_generator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TOTPGeneratorTest
{
	TEST_CLASS( TOTPGeneratorTest )
	{
	public:
		
		TEST_METHOD( Generate_ValidData )
		{
			TOTPGenerator generator;
			std::string secret = "helloworld";
			uint64_t timestamp = 332801291891769344; // 07.06.2023 16:22
			int tokenLength = 6;

			std::string expectedTOTP = "843383";

			std::string actualTOTP = generator.Generate( secret, timestamp, tokenLength );

			Assert::AreEqual( expectedTOTP, actualTOTP );
		}

		TEST_METHOD( Generate_WithRandomExpect_NotEqual ) {
			TOTPGenerator generator;
			std::string secret = "helloworld";
			uint64_t timestamp = 332801291891769344; // 07.06.2023 16:22
			int tokenLength = 6;

			std::string expectedTOTP = "888888";

			std::string actualTOTP = generator.Generate( secret, timestamp, tokenLength );

			Assert::AreNotEqual( expectedTOTP, actualTOTP );
		}
	};
}
