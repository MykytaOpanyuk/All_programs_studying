//#pragma once
#ifndef _Array_of_data_h_
#define _Array_of_data_h_
#include <vector>
#include "sorted_data.h"

static int new_arr_size;

template <class T>
class array_of_data
{
public:
	vector<sorted_data<T>> data;
	int size;
	array_of_data();
	~array_of_data();

	sorted_data<T> &operator[](int index);
	//void fill(int new_size);
	void add(sorted_data<T> elem);
	void print();
};

#include "array_of_data.cpp"

#endif