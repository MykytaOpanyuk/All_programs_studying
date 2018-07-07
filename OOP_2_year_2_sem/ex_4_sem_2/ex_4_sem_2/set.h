#pragma once
#include <iostream>
#include <vector>
#include "container_list.h"
using namespace std;

#define EPS 0.00001

template<class T>
class set_list {
public:
	container_list<T> cont;

	set_list() {};
	~set_list() {};

	int size()
	{
		return cont.len;
	}

	bool is_empty()
	{
		if (cont.head != NULL)
			return false;
		else
			return true;
	}

	void add(T data)
	{
		if (find(data) == NULL)
			cont.insert(cont.begin(), data);
	}

	void insert(int pos, T data)
	{
		if (pos > cont.len)
			cout << "Error, wrong position." << endl;
		else if (find(data) == NULL) {
			element<T> *timetable = cont.head->change();
			
			if (cont.head != NULL) {
				int i = 0;

				while ((i != pos) && (timetable->next != NULL)) {
					timetable = timetable->next;
					i++;
				}
			}

			cont.insert(timetable, data);
		}
	}
	void remove(T data)
	{
		element<T> *timetable = find(data);
		if (timetable != NULL)
			cont.erase(timetable);
	}

	element<T> *find(T data)
	{
		element<T> *timetable = cont.head->change();

		while (timetable != NULL) {
			if (fabs((double)timetable->data - (double)data) < EPS)
				return timetable;
			timetable = timetable->next;
		}
		return NULL;
	}
	
	void clear()
	{
		element<T> *timetable = cont.head->change();

		while (timetable != NULL) {
			element<T> *timetable1(timetable);
			timetable = timetable->next;
			cont.erase(timetable1);
		}
	}


	T first()
	{
		return cont.head->data;
	}
};

template<class T>
class set_vector {
public:
	vector<T> cont;

	set_vector() {};
	~set_vector() {};
	
	int size()
	{
		return cont.size();
	}

	bool is_empty()
	{
		if (cont.empty() == false)
			return false;
		else
			return true;
	}

	void add(T data)
	{
		if (find(data) == -1)
			cont.push_back(data);
	}

	void insert(unsigned int pos, T data)
	{
		if (pos > cont.size())
			cout << "Error, wrong position." << endl;
		else if (find(data) == -1) {
			typename vector<T>::iterator it = cont.begin();
			unsigned int i = 0;
			while (i < pos) {
				it++;
				i++;
			}
			cont.insert(it, data);
		}
	}

	void remove(T data)
	{
		int timetable = find(data);
		if (timetable != -1)
			cont.erase(cont.begin() + timetable);
	}

	int find(T data)
	{
		for (unsigned int i = 0; i < cont.size(); i++)
			if (fabs((double)cont[i] - (double)data) < EPS)
				return i;
		
		return -1;
	}

	void clear()
	{
		cont.clear();
	}

	T first()
	{
		if (cont.size() > 0)
			return cont[0];
	}
};