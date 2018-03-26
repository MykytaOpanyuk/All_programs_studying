#ifndef _Array_of_data_cpp_
#define _Array_of_data_cpp_
#include <vector>
#include "array_of_data.h"

template <class T>
array_of_data<T>::array_of_data()
{
	for (int i = 0; i < new_arr_size; i++)
		data.push_back(0);
	size = new_arr_size;
}

template <class T>
array_of_data<T>::~array_of_data()
{
	data.clear();
}
template <class T>
sorted_data<T> &array_of_data<T>::operator[](int index) 
{
	return data[index];
}

template <class T>
void array_of_data<T>::add(sorted_data<T> elem)
{
	data.push_back(elem);
}

template <class T>
void array_of_data<T>::print()
{
	for (int i = 0; i < size; i++) {
		data[i].print();
		cout << " ";
	}
}

#endif