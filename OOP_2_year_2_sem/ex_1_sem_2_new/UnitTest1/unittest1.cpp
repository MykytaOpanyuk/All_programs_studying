#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ex_1_sem_2_new/ex_1_sem_2.cpp"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			new_arr_size = 6;

			sortpack<int> sort_pack1(new_arr_size);
			sort_pack1.fill_test();
			//sort_pack1.show(sort_pack1.arr_reserve);
			sort_pack1.copy(sort_pack1.arr_reserve, sort_pack1.arr);
			//cout << "After copying!" << endl;
			//sort_pack1.show(sort_pack1.arr);
			sort_pack1.quick(0, (sort_pack1.arr.size - 1));
			sort_pack1.show(sort_pack1.arr_reserve);
			sort_pack1.show(sort_pack1.arr);
			//sorted_data<int> a = (sort_pack1.arr[0]);
			//sorted_data<int> b(0);

			sortpack<int> sort_pack2(new_arr_size);
			sort_pack2.copy(sort_pack1.arr_reserve, sort_pack2.arr_reserve);
			sort_pack2.copy(sort_pack2.arr_reserve, sort_pack2.arr);
			sort_pack2.bubble();
			Assert::AreEqual(sort_pack1.arr_reserve[0].data, sort_pack2.arr_reserve[0].data);
			Assert::AreEqual(sort_pack1.arr[0].data, sort_pack2.arr[0].data);
		}

	};
}