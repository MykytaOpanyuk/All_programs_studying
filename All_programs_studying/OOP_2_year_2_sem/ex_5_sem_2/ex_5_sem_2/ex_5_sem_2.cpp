// ex_5_sem_2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

template <class T>
class Stack
{
public:
	
	class element //class of element of stack
	{

	public:
		T data;
		element *next;
		
		element(const T &data);
		~element();

		const T &get_data();
		element *get_next();
		void set_next(element *new_next);
	};

	element *top_element;
	int size;

	Stack();
	~Stack();
	virtual bool empty();
	virtual const T &top();
	virtual void pop();
	virtual void push(const T &data);
};

template <class T>
Stack<T>::element::element(const T &new_data)
{
	data = T(new_data);
	next = NULL;
}

template <class T>
Stack<T>::element::~element()
{
	data.~T();
	next = NULL;
}

template <class T>
const T &Stack<T>::element::get_data() 
{
	return data;
}

template<class T>
typename Stack<T>::element *Stack<T>::element::get_next() 
{
	return next;
}

template<class T>
void Stack<T>::element::set_next(typename Stack<T>::element *new_next)
{
	next = new_next;
}

template<class T>
Stack<T>::Stack()
{
	top_element = NULL;
	size = 0;
}

template<class T>
Stack<T>::~Stack()
{
	while (size > 0)
		pop();
}

template<class T>
bool Stack<T>::empty()
{
	if (size == 0)
		return true;
	else
		return false;
}

template<class T>
const T &Stack<T>::top()
{
	if (!empty())
		return top_element->data;
	else
		cout << "Error, stack is empty!" << endl;
	return 0;
}

template<class T>
void Stack<T>::pop()
{
	if (!empty()) {
		element *to_pop = top_element;
		top_element = top_element->next;
		to_pop->~element();
		size--;
	}
	else
		cout << "Error, stack is empty!" << endl;
}

template<class T>
void Stack<T>::push(const T &data)
{
	element *prev = top_element;

	top_element = new element(data);
	top_element->set_next(prev);

	++size;
}

int main()
{
    return 0;
}

