#ifndef _Sorted_data_cpp_
#define _Sorted_data_cpp_

#include "sorted_data.h"

using namespace std;

template <class T>
sorted_data<T>::sorted_data()
{
	data = 0;
}

template <class T>
sorted_data<T>::sorted_data(T new_data)
{
	data = new_data;
}

template <class T>
void sorted_data<T>::print()
{
	cout << data;
}

template <class T>
bool sorted_data<T>::operator<(sorted_data<T> another_data)
{
	return (data < another_data.data);
}

template <class T>
T sorted_data<T>::get_data()
{
	return data;
}

template <class T>
void sorted_data<T>::set_data(T new_data)
{
	data = new_data;
}

#endif