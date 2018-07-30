#include "List.h"

class List;
class CyclicList: virtual public List
{
public:
	CyclicList(char elem):List(elem)
	{
		list_head = new Node;

		list_head->_elem = elem;
		list_head->_next = list_head;
	}

	virtual void cons(char elem) 
	{
		if (list_head == NULL)
			throw BadList();

		Node *timetable = list_head;
		Node *add = new Node;
		
		add->_elem = elem;

		while (timetable->_next != list_head) {
			timetable = timetable->_next;
		}

		timetable->_next = add;
		add->_next = list_head;
	}

	virtual void pop() 
	{
		if (list_head == NULL)
			throw BadList();

		Node *to_pop = list_head;
		Node *timetable = list_head;

		while (timetable->_next != list_head) {
			timetable = timetable->_next;
		}

		timetable->_next = list_head->_next;

		list_head = list_head->_next;

		delete to_pop;
	}

	virtual void show()
	{
		if (list_head == NULL)
			throw BadList();

		Node *timetable = list_head;

		while (timetable->_next != list_head) {
			timetable = timetable->_next;
		}
	}
};

class DoubleSidedList: virtual public List
{
public:
	Node *tail; // realise tail as another one sided list, which is reversed to head list

	DoubleSidedList(char elem):List(elem)
	{
		list_head = new Node;

		list_head->_elem = elem;
		list_head->_next = NULL;
		tail = new Node;
	}
	
	virtual void cons(char elem)
	{
		if (list_head == NULL)
			throw BadList();

		Node *timetable = list_head;
		Node *add = new Node;
		Node *add_for_tail = new Node;

		add->_elem = elem;
		add_for_tail->_elem = elem;

		do {
			add_for_tail->_next = timetable;
			timetable = timetable->_next;
		} while (timetable->_next != NULL);

		tail = add_for_tail;

		timetable->_next = add;
		add->_next = NULL;
	}

	virtual void pop()
	{
		if (tail == NULL)
			throw BadList();

		Node *to_pop_tail = tail;
		Node *timetable = to_pop_tail;

		while (to_pop_tail->_next != NULL) {
			timetable = to_pop_tail;
			to_pop_tail = to_pop_tail->_next;
		}
		delete to_pop_tail;
		timetable->_next = NULL;

		List::pop();
	}

};

class CyclicDoubleSidedList: public DoubleSidedList, public CyclicList
{
public:
	CyclicDoubleSidedList(char elem):DoubleSidedList(elem), CyclicList(elem), List(elem)
	{
		list_head = new Node;

		list_head->_elem = elem;
		list_head->_next = NULL;
		tail = new Node;
	}

	virtual void cons(char elem)
	{
		DoubleSidedList::cons(elem);
		
		Node *timetable = list_head;

		while (timetable->_next != NULL)
			timetable = timetable->_next;

		timetable->_next = list_head;

		timetable = tail;

		while (timetable->_next != NULL)
			timetable = timetable->_next;

		timetable->_next = tail;

	}

	virtual char head()
	{
		return List::head();
	}

	virtual void pop()
	{
		DoubleSidedList::pop();

		Node *timetable = list_head;

		while (timetable->_next != NULL)
			timetable = timetable->_next;

		timetable->_next = list_head;

		timetable = tail;

		while (timetable->_next != NULL)
			timetable = timetable->_next;

		timetable->_next = tail;
	}
};
