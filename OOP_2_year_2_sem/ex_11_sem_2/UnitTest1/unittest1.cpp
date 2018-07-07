#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ex_11_sem_2\calculator.h"
#include "..\ex_11_sem_2\calculator.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			calculator new_calculator_1("2+2-1*3");
			Assert::AreEqual(int(new_calculator_1.result->elem[0]), 1);
		}
		TEST_METHOD(TestMethod2)
		{
			calculator new_calculator_2("5^2/4-3");
			Assert::AreEqual(int(new_calculator_2.result->elem[0]), 3);
		}

	};
}