#pragma once
#include <iostream>

template<class T>
class element
{
public:
	T data;
	element<T> *next;
	element<T> *prev;

	element()
	{
		prev = NULL;
		next = NULL;
		data = 0;
	}

	element<T> *change()
	{
		//prev = another->prev;
		//next = another->next;
		//data = another->data;
		return this;
	}

	~element() {};

	void operator++()
	{
		data = data + 1;
	}
	bool operator!=(element<T> *another)
	{
		return (data != another->data);
	}
	bool operator==(element<T> *another)
	{
		return (data == another->data);
	}
};

template<class T>
class container_list
{

public:
	element<T> *head;
	int len;

	container_list()
	{
		len = 0;
		head = NULL;
	}
	/*
	~container_list()
	{
		while (head != NULL) {
			element<T> *timetable = head->change();
			head = head->next;
			delete timetable;
		}
	}
	*/
	element<T> *begin()
	{
		return head;
	}

	element<T> *end()
	{
		element<T> *timetable = head->change();

		while (timetable->next != NULL) {
			timetable = timetable->next;
		}

		return timetable;
	}

	void insert(element<T> *insert_pos, T data)
	{
		element<T> *new_element = new element<T>;

		new_element->data = data;
		len++;
		if (head == NULL)
			head = new_element->change();
		else {
			if (insert_pos == head)
				head = new_element->change();
			
			if (insert_pos->prev != NULL) {
				insert_pos->prev->next = new_element;
				new_element->prev = insert_pos->prev;
			}

			new_element->next = insert_pos;
			insert_pos->prev = new_element;
		}
	}
	void erase(element<T> *to_delete)
	{
		if (to_delete->next != NULL)
			to_delete->next->prev = to_delete->prev;
		if (to_delete->prev != NULL)
			to_delete->prev->next = to_delete->next;
		delete to_delete;
		len--;
	}

	int size()
	{
		return len;
	}
};


