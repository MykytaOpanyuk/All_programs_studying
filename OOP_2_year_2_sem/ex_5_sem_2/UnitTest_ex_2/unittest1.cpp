#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ex_5_sem_2/ex_5_sem_2.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_ex_2
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Stack<int> new_stack;
			new_stack.push(1);
			new_stack.push(5);
			new_stack.push(3);

			Stack<int>::element *el = new_stack.top_element;

			Assert::AreEqual(el->data, 3);

			Assert::AreEqual(el->next->data, 5);

			Assert::AreEqual(el->next->next->data, 1);
			
			new_stack.pop();
			el = new_stack.top_element;
			Assert::AreEqual(el->data, 5);
		}
		TEST_METHOD(TestMethod2)
		{
			Stack<double> new_stack_2;
			new_stack_2.push(1.5);
			new_stack_2.push(10.2);
			new_stack_2.push(16.6);

			Stack<double>::element *el2 = new_stack_2.top_element;

			Assert::AreEqual(el2->data, 1.5);

			Assert::AreEqual(el2->next->data, 10.2);

			Assert::AreEqual(el2->next->next->data, 16.6);

			new_stack_2.pop();
			el2 = new_stack_2.top_element;
			Assert::AreEqual(el2->data, 10.2);
		}
	};
}