#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ex_4_sem_2\ex_4_sem_2.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			///*
			set_list<int> new_set1;
			new_set1.insert(0, 15);
			Assert::AreEqual(new_set1.cont.head->data, 15);
			new_set1.add(54);
			Assert::AreEqual(new_set1.cont.head->data, 54);
			new_set1.add(66);
			Assert::AreEqual(new_set1.cont.head->data, 66);
			new_set1.insert(1, 5);
			Assert::AreEqual(new_set1.cont.head->next->data, 5);
			new_set1.remove(5);
			Assert::AreEqual(new_set1.cont.head->next->data, 54);
			new_set1.add(13);
			new_set1.clear();
			//*/

			set_vector<int> new_set2;
			new_set2.insert(0, 15);
			Assert::AreEqual(new_set2.cont[0], 15);
			new_set2.add(54);
			Assert::AreEqual(new_set2.cont[1], 54);
			new_set2.add(66);
			Assert::AreEqual(new_set2.cont[2], 66);
			new_set2.insert(1, 5);
			Assert::AreEqual(new_set2.cont[1], 5);
			new_set2.remove(5);
			Assert::AreEqual(new_set2.cont[1], 54);
			new_set2.add(13);
			Assert::AreEqual(new_set2.cont[3], 13);
			new_set2.clear();

		}

	};
}