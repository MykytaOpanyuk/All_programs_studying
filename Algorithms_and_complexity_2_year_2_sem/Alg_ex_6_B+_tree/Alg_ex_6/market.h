#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

typedef enum { Black, Red } node_colour; 

struct product_element;

struct type_element {
	string group;
	vector<product_element *> array_of_products;
};

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
	vector<type_element *> type_of_products;
	vector<product_element *> array_of_products;

	market();
	~market();
	int check_is_product(int pos, string product);
	int check_is_type(string product);
	product_element *new_data();
	string delete_data();
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
			delete_data();
		}		

	} while (choice != 3);
	save_changing();
}

market::~market()
{
	type_of_products.clear();
	array_of_products.clear();
}

int market::check_is_product(int pos, string product)
{
	if (pos == -1)
		return -1;
	for (unsigned int i = 0; i < type_of_products[pos]->array_of_products.size(); i++)
		if (type_of_products[pos]->array_of_products[i]->name == product)
			return i;
	return -1;
}

int market::check_is_type(string type_group)
{

	for (unsigned int i = 0; i < type_of_products.size(); i++)
		if (type_of_products[i]->group == type_group)
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

	int i = check_is_type(new_group);
	int pos = check_is_product(i, new_name);
	
	if (i == -1) {
		size_of_f++;
		type_of_products.push_back(new type_element);

		unsigned int size = unsigned int(type_of_products.size() - 1);

		type_of_products[size]->array_of_products.push_back(new product_element);

		type_of_products[size]->group = new_group;

		type_of_products[size]->array_of_products[type_of_products[size]->array_of_products.size() - 1]->name = new_name;
		type_of_products[size]->array_of_products[type_of_products[size]->array_of_products.size() - 1]->key =
			string_to_key(new_name);
		cout << "Write a price of product:" << endl;
		cin >> type_of_products[size]->array_of_products[type_of_products[size]->array_of_products.size() - 1]->price;
		type_of_products[size]->array_of_products[type_of_products[size]->array_of_products.size() - 1]->group = new_group;
		
		array_of_products.push_back(type_of_products[size]->array_of_products[type_of_products[size]->array_of_products.size() - 1]);
		cout << "New product was added." << endl;
		return array_of_products[array_of_products.size() - 1];
	}

	else if (pos == -1) {
		size_of_f++;
		type_of_products[i]->array_of_products.push_back(new product_element);

		unsigned int size = unsigned int(type_of_products[i]->array_of_products.size()) - 1;

		type_of_products[i]->array_of_products[size]->name = new_name;
		type_of_products[i]->array_of_products[size]->key = string_to_key(new_name);

		cout << "Write a price of product:" << endl;
		cin >> type_of_products[i]->array_of_products[size]->price;
		type_of_products[i]->array_of_products[size]->group = new_group;
		array_of_products.push_back(type_of_products[i]->array_of_products[size]);
		cout << "New product was added." << endl;
		return array_of_products[array_of_products.size() - 1];
	}
	return NULL;
}

string market::delete_data()
{
	string type_p, product_p;
	int i, pos;

	cout << "Write a group of product: " << endl;
	cin.get();
	getline(cin, type_p);

	i = check_is_type(type_p);
	if (i == -1)
		return "";

	cout << "Write a name of product: " << endl;
	getline(cin, product_p);

	pos = check_is_product(i, product_p);
	if (pos == -1)
		return "";

	string to_return = type_of_products[i]->array_of_products[pos]->name;

	unsigned int size = unsigned int(type_of_products[i]->array_of_products.size());

	type_of_products[i]->array_of_products.erase(type_of_products[i]->array_of_products.begin() + pos);
	if (size == 1)
		type_of_products.erase(type_of_products.begin() + i);
	
	return to_return;
}

void market::save_changing()
{
	fstream file("products.txt", ios::out);
	file << size_of_f << "\n";
	for (unsigned int i = 0; i < type_of_products.size(); i++)
		for (unsigned int j = 0; j < type_of_products[i]->array_of_products.size(); j++) {
			file << type_of_products[i]->array_of_products[j]->group << ";";
			file << type_of_products[i]->array_of_products[j]->name << ";";
			file << type_of_products[i]->array_of_products[j]->price << "\n";
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

	if (type_of_products.size() == 0)
		while (j != size_of_f) {
			product_element *value = new product_element;
			getline(file, value->group, ';');
			getline(file, value->name, ';');
			file >> value->price;
			file.get(c); // reading \n
			value->key = string_to_key(value->name);
			int i = check_is_type(value->group);
			if (i != -1) {
				type_of_products[i]->array_of_products.push_back(value);
				array_of_products.push_back(value);
			}
			else {
				type_of_products.push_back(new type_element);
				unsigned int size = unsigned int(type_of_products.size()) - 1;
				type_of_products[size]->group = value->group;
				type_of_products[size]->array_of_products.push_back(value);
				array_of_products.push_back(value);
			}
			j++;
		}
	file.close();
}

market *main_data;