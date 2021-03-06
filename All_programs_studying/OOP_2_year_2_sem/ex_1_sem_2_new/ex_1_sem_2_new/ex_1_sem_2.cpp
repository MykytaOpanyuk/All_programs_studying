#include "array_of_data.h"
#include "sorted_data.h"
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

template <class T>
class sortpack {

public:
	int quantity;
	int counter;
	array_of_data<T> arr; //for sorting
	array_of_data<T> arr_reserve; //not sorted array

	sortpack(int new_size);
	void counterIncrease();
	void counterReset();
	void show(array_of_data<T> a);
	void fill_by_user();
	void fill_test();
	void my_swap(sorted_data<T> &a, sorted_data<T> &b);
	void copy(array_of_data<T> from, array_of_data<T> &to);
	void quick(int left, int right);
	void shell();
	void select();
	void bubble();
	void insert();

};

template <class T>
sortpack<T>::sortpack(int new_size)
{
	new_arr_size = new_size;
	counterReset();
}

template <class T>
void sortpack<T>::counterIncrease()
{
	++counter;
}

template <class T>
void sortpack<T>::counterReset() 
{
	counter = 0;
}

template <class T>
void sortpack<T>::show(array_of_data<T> a)
{
	a.print();
	cout << endl;
}

template <class T>
void sortpack<T>::fill_by_user()
{
	cout << "write different numbers " << arr_reserve.size << " time" << endl;
	for (int i = 0; i < arr_reserve.size; i++) {
		T new_data;
		cin >> new_data;
		arr_reserve[i] = sorted_data<T>(new_data); //using another constructor
	}
}

template <class T>
void sortpack<T>::fill_test() 
{
	srand(time(NULL));
	//cout << "write different numbers " << arr_reserve.size << " time" << endl;
	for (int i = 0; i < arr_reserve.size; i++) {
		T new_data = rand() % 1000;
		arr_reserve[i] = sorted_data<T>(new_data); //using another constructor
	}
}

template <class T>
void sortpack<T>::my_swap(sorted_data<T> &a, sorted_data<T> &b)
{
	T c = a.get_data();
	a.set_data(b.data);
	b.set_data(c);
}

template <class T>
void sortpack<T>::copy(array_of_data<T> from, array_of_data<T> &to)
{
	if (from.size != to.size) {
		cout << "Error, different size." << endl;
		return;
	}
	
	for (int i = 0; i < from.size; i++)
		to[i] = from[i];
}

template <class T>
void sortpack<T>::quick(int left, int right)
{
	if (right == -1) 
		right = arr.size - 1;
	
	if (left >= right) 
		return;

	int i = left, j = right;

	sorted_data<T> &pivot = arr[((left + right) / 2)];

	while (i <= j) {
		while (arr[i] < pivot)
			i++;
		while (pivot < arr[j])
			j--;
		if (i <= j) {
			my_swap(arr[i], arr[j]);
			i++;
			j--;
		}
	};

	if (left < j)
		quick(left, j);
	if (i < right)
		quick(i, right);
}

template <class T>
void sortpack<T>::shell()
{
	for (int step = arr.size/2; step > 0; step = step/2)
		for (int i = step; i < n; i++) {
			sorted_data<T> & temp = arr[i];
			int j;
			for (j = i; ((j >= step) && (temp < arr[j])); j = j - step)
				arr[j] = arr[j - step];
			arr[j] = temp;
		}

}

template <class T>
void sortpack<T>::select() 
{
	int i = 0;

	while (i < arr.size) {
		int min = i;

		for (int j = min; j < arr.size; j++)
			if (arr[j] < arr[min])
				min = j;

		if (min > i) {
			my_swap(arr[i], arr[min]);
			counterIncrease();
		}
		i++;
	}
}

template <class T>
void sortpack<T>::bubble() 
{
	int k = arr.size - 1;
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k - i; j++) {
			if (arr[j + 1] < arr[j]) {
				my_swap(arr[j], arr[j + 1]);
				counterIncrease();
			}
		}
}

template <class T>
void sortpack<T>::insert() 
{
	for (int i = 1; i < arr.size; i++) {
		sorted_data<T> &key = arr[i];
		int j = i - 1;
		while ((j >= 0) && (key < arr[j])) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
}

int main()
{
	cout << "Write a size of array :" << endl;
	cin >> new_arr_size;

	sortpack<int> sort_pack1(new_arr_size);
	sort_pack1.fill_by_user();
	sort_pack1.show(sort_pack1.arr_reserve);
	sort_pack1.copy(sort_pack1.arr_reserve, sort_pack1.arr);
	cout << "After copying!" << endl;
	sort_pack1.show(sort_pack1.arr);
	sort_pack1.insert();
	sort_pack1.show(sort_pack1.arr_reserve);
	sort_pack1.show(sort_pack1.arr);
	sorted_data<int> a = (sort_pack1.arr[0]);
	sorted_data<int> b(0);
	system("pause");
	return 0;
}

