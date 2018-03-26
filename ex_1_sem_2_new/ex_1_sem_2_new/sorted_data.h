//#pragma once
#ifndef _Sorted_data_h_
#define _Sorted_data_h_

#include <iostream>

template <class T>
class sorted_data
{
public:
	T data;
	sorted_data();
	sorted_data(T new_data);
	~sorted_data() {};
	bool operator<(sorted_data<T> another_data);
	void print();

	T get_data();
	void set_data(T new_data);
};

#include "sorted_data.cpp"

#endif