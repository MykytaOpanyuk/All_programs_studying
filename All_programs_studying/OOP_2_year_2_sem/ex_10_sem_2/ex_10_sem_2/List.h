#include <iostream>
#include <exception>
#include "ListIterator.h"
using namespace std;



//--------------List---------------//
class List
{

public:
	struct Node
	{
		char _elem;
		Node * _next;
	};

	Node *list_head;

	//----------List iterator----------//
	class ListIterator
	{


		bool start;
		List::Node * _first;
		List::Node * _current;
	public:
		void begin(List* data)
		{
			_first = data->list_head;
			_current = data->list_head;
			start = true;
		}

		char get()
		{
			return _current->_elem;
		}

		void next()
		{
			if (end() != true) {
				_current = _current->_next;
				start = false;
			}
		}
		bool end()
		{
			if (_current == NULL)
				return true;
			return false;
		}
	};
	//----------List iterator----------//


	class BadList : public exception {
	public:
		const char *what() 
		{
			return ("Head is null!");
		}
		void message() 
		{
			cout << what();
		}
	};

	List(char elem) 
	{
		list_head = new Node;

		list_head->_elem = elem;
		list_head->_next = NULL;
	}

	virtual void cons(char elem) // add to end of list
	{
		if (list_head == NULL)
			throw BadList();

		Node *add = new Node;
		Node *timetable = list_head;
		add->_elem = elem;

		while (timetable->_next != NULL)
			timetable = timetable->_next;

		timetable->_next = add;
	}

	virtual char head()
	{
		if (list_head == NULL) 
			throw BadList();

		return (list_head->_elem);
	}

	virtual void pop()
	{
		if (list_head == NULL)
			throw BadList();

		Node *to_pop = list_head;
		list_head = list_head->_next;

		delete to_pop;
	}

	virtual ListIterator iterate()
	{
		ListIterator *data = new ListIterator();
		data->begin(this); //this = list
		return *data;
	}
	virtual void show()
	{
		if (list_head == NULL)
			throw BadList();

		Node *timetable = list_head;

		while (timetable->_next != NULL) {
			cout << timetable->_elem << " ";
			timetable = timetable->_next;
		}
		cout << endl;
	}

};
//--------------List---------------//