#include "lists.h"

void listTest() {
	cout << "------------- Simple list test -------------" << endl;
	List myList('a');

	// Add chars to list
	for (char i = 'b'; i != 'f'; i++) {
		myList.cons(i);
	}
	
	// Print list using "show" method
	cout << "Print list by method: "; myList.show();

	
	cout << "Print list by iterator: ";
	for (auto i = myList.iterate(); !i.end(); i.next()) {
		cout << i.get() << " ";
	}
	cout << endl;

	cout << "Deleteing list" << endl;
	while (true) {
		try {
			myList.pop();
		}
		catch(List::BadList e) {
			cout << "Exception caught: list deleted" << endl;
			break;
		}
	}

}
void cyclicListTest() {
	cout << "------------- Cyclic list test -------------" << endl;
	
	CyclicList myList('a');
	for (char i = 'b'; i != 'f'; i++) {
		myList.cons(i);
	}

	cout << "Print list by method: "; myList.show();

	cout << "Deleteing list" << endl;
	while (true) {
		try {
			myList.pop();
		}
		catch (CyclicList::BadList e) {
			cout << "Exception caught: list deleted" << endl;
			break;
		}
	}
}
void doubleSidedListTest() {
	cout << "---------- Double-sided list test ----------" << endl;
	DoubleSidedList myList('a');

	// Add chars to list
	for (char i = 'b'; i != 'f'; i++) {
		myList.cons(i);
	}
	
	// Print list using "show" method
	cout << "Print list by method: "; myList.show();

	
	cout << "Print list by iterator: ";
	for (auto i = myList.iterate(); !i.end(); i.next()) {
		cout << i.get() << " ";
	}
	cout << endl;

	cout << "Deleteing list" << endl;
	while (true) {
		try {
			myList.pop();
		}
		catch(List::BadList e) {
			cout << "Exception caught: list deleted" << endl;
			break;
		}
	}
}
void cyclicDoubleSidedListTest() {
	cout << "------- Cyclic Double-sided list test -------" << endl;
	CyclicDoubleSidedList myList('a');

	// Add chars to list
	for (char i = 'b'; i != 'f'; i++) {
		myList.cons(i);
	}
	
	// Print list using "show" method
	cout << "Print list by method: "; myList.show();

	cout << "Deleteing list" << endl;
	while (true) {
		try {
			myList.pop();
		}
		catch(List::BadList e) {
			cout << "Exception caught: list deleted" << endl;
			break;
		}
	}
}

int main() {
	//listTest(); cout << endl;
	cyclicListTest(); cout << endl;
	//doubleSidedListTest(); cout << endl;
	//cyclicDoubleSidedListTest(); cout << endl;

	system("Pause");

}