// ex_4_sem_2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "set.h"

int main()
{
	set_list<double> new_set1;
	new_set1.insert(0, 15 / 2);
	new_set1.add(54 / 7);
	new_set1.add(66.2);
	new_set1.insert(1, 5 / 2);
	new_set1.remove(5 / 2);
	new_set1.add(13.232);
	new_set1.clear();

	set_vector<double> new_set2;
	new_set2.insert(0, 15 / 2);
	new_set2.add(54 / 7);
	new_set2.add(66.2);
	new_set2.insert(1, 5 / 2);
	new_set2.remove(5 / 2);
	new_set2.add(13.232);
	new_set2.clear();
	system("pause");
	return 0;
}

