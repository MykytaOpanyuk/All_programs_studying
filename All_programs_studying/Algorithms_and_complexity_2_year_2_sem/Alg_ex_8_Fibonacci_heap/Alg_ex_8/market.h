#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct product_element {
	string group;
	string name;
	double price;
	int key;
};

int string_to_key(string name)
{
	int c = 0;

	for (unsigned int i = 0; i < name.size(); i++)
		c = c + name[i];

	return c;
}

bool is_empty_file(fstream &file)
{
	return (file.peek() == ifstream::traits_type::eof());
}

class market
{

public:
	unsigned int size_of_f;
	vector<product_element *> array_of_products;

	market();
	~market();
	int check_is_product(string product);
	product_element *new_data();
	string delete_data(string product_p);
	void save_changing();
	void read_data_from_file();
};

market::market()
{
	int choice;//number of elements


	do {
		cout << "If file is not empty, reading data from file!" << endl;
		read_data_from_file();
		cout << "1)Add new data." << endl;
		cout << "2)Delete data." << endl;
		cout << "3)Use this data." << endl;
		cin >> choice;
		if (choice == 1)
			new_data();
		if (choice == 2) {
			string name_p;

			cout << "write name of product :" << endl;
			cin.get();
			getline(cin, name_p);
			if (check_is_product(name_p) != -1)
				delete_data(name_p);
		}

	} while (choice != 3);
	save_changing();
}

market::~market()
{

	array_of_products.clear();
}

int market::check_is_product(string product)
{
	for (unsigned int i = 0; i < array_of_products.size(); i++)
		if (array_of_products[i]->name == product)
			return i;
	return -1;
}

product_element *market::new_data()
{
	bool check = true;
	string new_group, new_name;

	cout << "Write a group of product: " << endl;

	cin.get();
	getline(cin, new_group);

	cout << "Write a name of product: " << endl;

	getline(cin, new_name);

	int pos = check_is_product(new_name);

	if (pos == -1) {
		size_of_f++;

		product_element *new_element = new product_element;

		new_element->group = new_group;
		new_element->name = new_name;


		new_element->key = string_to_key(new_name);
		cout << "Write a price of product:" << endl;
		cin >> new_element->price;

		array_of_products.push_back(new_element);
		cout << "New product was added." << endl;
		return array_of_products[array_of_products.size() - 1];
	}

	return NULL;
}

string market::delete_data(string product_p)
{
	int pos;

	pos = check_is_product(product_p);
	if (pos == -1)
		return "";

	string to_return = array_of_products[pos]->name;

	unsigned int size = unsigned int(array_of_products.size());

	array_of_products.erase(array_of_products.begin() + pos);

	size_of_f--;
	return to_return;
}

void market::save_changing()
{
	fstream file("products.txt", ios::out);
	file << size_of_f << "\n";
	for (unsigned int i = 0; i < array_of_products.size(); i++) {
		file << array_of_products[i]->group << ";";
		file << array_of_products[i]->name << ";";
		file << array_of_products[i]->price << "\n";
	}
	file.close();
}

void market::read_data_from_file()
{

	fstream file("products.txt", ios::in);
	if (!file)
		return;

	if (is_empty_file(file))
		return;

	char c;
	int j = 0;

	file >> size_of_f;
	file.get(c);


	while (j != size_of_f) {
		product_element *value = new product_element;
		getline(file, value->group, ';');
		getline(file, value->name, ';');
		file >> value->price;
		file.get(c); // reading \n
		value->key = string_to_key(value->name);

		if (check_is_product(value->name) == -1)
			array_of_products.push_back(value);

		j++;
	}
	file.close();
}

market *main_data;