#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include "../ex_10_sem_2/Lists.h"
#include "../ex_10_sem_2/ListIterator.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			List new_list('a');

			for (char i = 'b'; i != 'ñ'; i++) {
				new_list.cons(i);
			}

			Assert::AreEqual(new_list.list_head->_elem, 'a');
			Assert::AreEqual(new_list.list_head->_next->_elem, 'b');
			Assert::AreEqual(new_list.list_head->_next->_next->_elem, 'c');


			new_list.pop();

			Assert::AreEqual(new_list.list_head->_elem, 'b');

			while (true) {
				try {
					new_list.pop();
				}
				catch (List::BadList e) {
					cout << "Exception caught: list deleted" << endl;
					break;
				}
			}

		}

		TEST_METHOD(TestMethod2)
		{
			CyclicList new_list('a');

			for (char i = 'b'; i != 'ñ'; i++) {
				new_list.cons(i);
			}

			Assert::AreEqual(new_list.list_head->_elem, 'a');
			Assert::AreEqual(new_list.list_head->_next->_elem, 'b');
			Assert::AreEqual(new_list.list_head->_next->_next->_elem, 'c');
			Assert::AreEqual(new_list.list_head->_next->_next->_next->_elem, 'a');


			new_list.pop();

			Assert::AreEqual(new_list.list_head->_elem, 'b');

			while (true) {
				try {
					new_list.pop();
				}
				catch (List::BadList e) {
					cout << "Exception caught: list deleted" << endl;
					break;
				}
			}
		}

		TEST_METHOD(TestMethod3)
		{
			DoubleSidedList new_list('a');

			for (char i = 'b'; i != 'ñ'; i++) {
				new_list.cons(i);
			}

			Assert::AreEqual(new_list.list_head->_elem, 'a');
			Assert::AreEqual(new_list.list_head->_next->_elem, 'b');
			Assert::AreEqual(new_list.list_head->_next->_next->_elem, 'c');

			Assert::AreEqual(new_list.tail->_elem, 'c');
			Assert::AreEqual(new_list.tail->_next->_elem, 'b');
			Assert::AreEqual(new_list.tail->_next->_next->_elem, 'a');

			new_list.pop();

			Assert::AreEqual(new_list.list_head->_elem, 'b');

			while (true) {
				try {
					new_list.pop();
				}
				catch (List::BadList e) {
					cout << "Exception caught: list deleted" << endl;
					break;
				}
			}
		}

		TEST_METHOD(TestMethod4)
		{
			DoubleSidedList new_list('a');

			for (char i = 'b'; i != 'ñ'; i++) {
				new_list.cons(i);
			}

			Assert::AreEqual(new_list.list_head->_elem, 'a');
			Assert::AreEqual(new_list.list_head->_next->_elem, 'b');
			Assert::AreEqual(new_list.list_head->_next->_next->_elem, 'c');
			Assert::AreEqual(new_list.list_head->_next->_next->_next->_elem, 'a');

			Assert::AreEqual(new_list.tail->_elem, 'c');
			Assert::AreEqual(new_list.tail->_next->_elem, 'b');
			Assert::AreEqual(new_list.tail->_next->_next->_elem, 'a');
			Assert::AreEqual(new_list.list_head->_next->_next->_next->_elem, 'c');

			new_list.pop();

			Assert::AreEqual(new_list.list_head->_elem, 'b');

			while (true) {
				try {
					new_list.pop();
				}
				catch (List::BadList e) {
					cout << "Exception caught: list deleted" << endl;
					break;
				}
			}

		};
	};
};