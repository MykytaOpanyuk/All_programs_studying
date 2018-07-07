//============================================================================
// Name        : perfect_hasing_ex_1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <list>
#include <cmath>
#include <ctime>
#include <string>

using namespace std;

#define NMAX 1000

const int check_collision_value = -1; //use it to check collision after hashing
unsigned int p;//prime number for hashing function

struct product_element;

struct type {
	string group;
	unsigned int hash_key;
	vector<product_element *> array_of_products;
};
struct product_element {
	string group;
	string name;
	double price;
	unsigned int hash_key;
	//type *parent;
};

struct for_hashing {
	unsigned int a;
	unsigned int b;
	int m;
};
template <class T>
unsigned int max_number(vector<T *> tarray)
{
	unsigned int max_data = 0;
	for (unsigned int i = 0; i < (unsigned int)tarray.size(); i++) {
		if (max_data < tarray[i]->hash_key)
			max_data = tarray[i]->hash_key;
	}
	return max_data;
}

bool is_prime(unsigned int value)
{
	if ((value == 0) || (value == 1))
		return false;

	for (unsigned int i = 2; i <= (unsigned int)sqrt(value); i++)
		if (value % i == 0)
			return false;

	return true;
}

unsigned int successor_prime(unsigned int value)
{
	if (value == 0)
		value++;
	value++;
	while (!is_prime(value))
		value++;

	return value;
}

unsigned int hash_number(for_hashing *table, int key)
{
	return (((table->a * key + table->b) % p) % table->m);
}

unsigned int hash_string(string key)
{
	unsigned int hash = 0;
    int c = 0;

    for (unsigned int i = 0; i < key.size(); i++)
    	c = c + key[i];
        hash = (hash * 10) + c;

    return hash;
}

bool is_empty_file(fstream &file) 
{
	return (file.peek() == ifstream::traits_type::eof());
}

class products
{

public :
	unsigned int size_of_f;
	vector<type *> type_of_products;
	//vector<product_element *> array_of_products;
	int check_is_product(int pos, string product);
	int check_is_type(string product);
	void new_data();
	void delete_data(product_element *name);
	void save_changing();
	void read_data_from_file();
};

int products::check_is_product(int pos, string product)
{
	if (pos == -1)
		return -1;
	for (unsigned int i = 0; i < type_of_products[pos]->array_of_products.size(); i++)
		if (type_of_products[pos]->array_of_products[i]->name == product)
			return i;
	return -1;
}

int products::check_is_type(string type_group)
{

	for (unsigned int i = 0; i < type_of_products.size(); i++)
		if (type_of_products[i]->group == type_group)
			return i;
	return -1;
}

void products::new_data()
{
	bool check = true;
	char answer;

	while (check) {
		string new_group, new_name;

		cout << "Write a group of product: " << endl;

		cin.get();
		getline(cin, new_group);

		cout << "Write a name of product: " << endl;
		//cin.get();
		getline(cin, new_name);

		int i = check_is_type(new_group);
		int pos = check_is_product(i, new_group);
		if (i != -1) {
			size_of_f++;
			type_of_products[i]->array_of_products.push_back(new product_element);

			unsigned int size = unsigned int(type_of_products[i]->array_of_products.size()) - 1;

			type_of_products[i]->array_of_products[size]->name = new_name;

			cout << "Write a price of product:" << endl;
			cin >> type_of_products[i]->array_of_products[size]->price;
			type_of_products[i]->array_of_products[size]->group = new_group;
			type_of_products[i]->array_of_products[size]->hash_key = hash_string(new_name);
		}
		else if (pos == -1) {
			size_of_f++;
			type_of_products.push_back(new type);

			unsigned int size = unsigned int(type_of_products.size() - 1);

			type_of_products[size]->array_of_products.push_back(new product_element);

			type_of_products[size]->group = new_group;
			type_of_products[size]->hash_key = hash_string(type_of_products[size]->group);

			type_of_products[size]->array_of_products[type_of_products[size]->array_of_products.size() - 1]->name = new_name;

			cout << "Write a price of product:" << endl;
			cin >> type_of_products[size]->array_of_products[type_of_products[size]->array_of_products.size() - 1]->price;
			type_of_products[size]->array_of_products[type_of_products[size]->array_of_products.size() - 1]->group = new_group;
			type_of_products[size]->array_of_products[type_of_products[size]->array_of_products.size() - 1]->hash_key = hash_string(new_name);
		}
		else
			cout << "New product was added." << endl;
		cout << "Do you want add another product ? Y or N" << endl;
		cin >> answer;
		if (answer == 'N')
			check = false;
	}
}

void products::delete_data(product_element *product)
{
	int i = check_is_type(product->group);
	int pos = check_is_product(i, product->name);
	unsigned int size = unsigned int(type_of_products[i]->array_of_products.size());

	type_of_products[i]->array_of_products.erase(type_of_products[i]->array_of_products.begin() + pos);
	if (size == 1)
		type_of_products.erase(type_of_products.begin() + i);
}

void products::save_changing()
{
	fstream file("products.txt", ios::out);
	file << size_of_f << "\n";
	for (unsigned int i = 0; i < type_of_products.size(); i++)
		for(unsigned int j = 0; j < type_of_products[i]->array_of_products.size(); j++) {
			file << type_of_products[i]->array_of_products[j]->group << ";";
			file << type_of_products[i]->array_of_products[j]->name << ";";
			file << type_of_products[i]->array_of_products[j]->price << " ";
			file << type_of_products[i]->array_of_products[j]->hash_key << "\n";
		}
	file.close();
}

void products::read_data_from_file()
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
			file >> value->hash_key;
			file.get(c); // reading \n
			int i = check_is_type(value->group);
			if (i != -1) {
				type_of_products[i]->array_of_products.push_back(value);
			}
			else {
				type_of_products.push_back(new type);
				unsigned int size = unsigned int(type_of_products.size()) - 1;
				type_of_products[size]->group = value->group;
				type_of_products[size]->hash_key = hash_string(value->group);
				type_of_products[size]->array_of_products.push_back(value);
			}
			j++;
		}
	file.close();
}


class secondary_table
{
public:
	for_hashing *new_table;
	vector<unsigned int> cells;
	secondary_table() {};
	void Initialize_variables(int size);
	void Construct_for_types(vector<unsigned int> types);
	void Construct_for_products(vector<unsigned int> products);

};

void secondary_table::Initialize_variables(int size)
{
	if (size == 1) {
		new_table->m = size;
		new_table->a = 0;
		new_table->b = 0;
	}
	else {
		new_table->m = size;
		new_table->a = rand() % p;
		new_table->b = rand() % (p - 1) + 1;
	}
	for (int i = 0; i < size; i++)
		cells[i] = check_collision_value;
}

void secondary_table::Construct_for_types(vector<unsigned int> types)
{
	unsigned int size;
	unsigned int hash_key;


	if (types.empty()) {
		size = 0;
		return;
	}
	size = unsigned int (types.size() * types.size());//to a have a low chance of collision

	bool check = false;

	cells.resize(size);
	for (int i = 0; i < size; i++)
		cells.push_back(0);

	new_table = new for_hashing;
	// while there is no collisions in table
	while (check == false) {
		Initialize_variables(size);

		check = true;

	    for (unsigned int i = 0; i < types.size(); i++) {
	    	hash_key = hash_number(new_table, types[i]);
	    	// if collision then try construct new hash table
			if (cells[hash_key] != check_collision_value) {
				check = false;
				break;
			}
	    	cells[hash_key] = types[i];
	    }
		if (check == true)
			for (unsigned int i = 0; i < size; i++)
				cout << cells[i] << " ";
	}
}

void secondary_table::Construct_for_products(vector<unsigned int> products)
{
	unsigned int size;
	unsigned int hash_key;


	if (products.empty()) {
		size = 0;
		return;
	}
	size = unsigned int(products.size() * products.size());//to a have a low chance of collision

	bool check = false;

	cells.resize(size);
	for (int i = 0; i < size; i++)
		cells.push_back(0);

	new_table = new for_hashing;
	// while there is no collisions in table
	while (check == false) {
		Initialize_variables(size);

		check = true;

	    for (unsigned int i = 0; i < products.size(); i++) {
	    	hash_key = hash_number(new_table, products[i]);
	    	// if collision then try construct new hash table
			if (cells[hash_key] != check_collision_value) {
				check = false;
				break;
			}
	    	cells[hash_key] = products[i];
	    }

		if (check == true)
			for (unsigned int i = 0; i < size; i++)
				cout << cells[i] << " ";
	}
}

class main_table
{
	for_hashing *basic_table;
    vector<vector<unsigned int> > elements_in_cells;
    vector<secondary_table> sec_tables;

public:
    main_table(unsigned int size);
    void Initialize_types(vector<type *>& types);
    void Initialize_products(vector<product_element *>& products);
    bool contains_type(string type);
    bool contains_product(string product);
};

main_table::main_table(unsigned int size)
{
    basic_table = new for_hashing;
    basic_table->m = size;
    basic_table->a = rand() % p;
    basic_table->b = rand() % (p - 1) + 1;

    sec_tables.resize(size);
}

void main_table::Initialize_types(vector<type *>& types) //type of data for hashing is numbers
{
	elements_in_cells.resize(basic_table->m);
    for (unsigned int i = 0; i < types.size(); i++) {
		unsigned int hash_key = hash_number(basic_table, types[i]->hash_key);
		elements_in_cells[hash_key].push_back(types[i]->hash_key);
		cout << " Hash key : " << hash_key << " -- Added " << types[i]->group << " /";
    }
	cout << "." << endl;
    for (unsigned int i = 0; i < types.size(); i++) {
		cout << "Hash key : " << i << " Second table have keys : ";
    	sec_tables[i].Construct_for_types(elements_in_cells[i]);
		cout << endl;
    }

}

void main_table::Initialize_products(vector<product_element *>& products) //type of data for hashing is numbers
{
	elements_in_cells.resize(basic_table->m);

    for (unsigned int i = 0; i < products.size(); i++) {
		unsigned int hash_key = hash_number(basic_table, products[i]->hash_key);
        elements_in_cells[hash_key].push_back(products[i]->hash_key);
		cout << " Hash key : " << hash_key << " -- Added " << products[i]->group << " /";
    }
	cout << "." << endl;
    for (unsigned int i = 0; i < products.size(); i++) {
		cout << "Hash key : " << i << " / Second table have keys : ";
    	sec_tables[i].Construct_for_products(elements_in_cells[i]);
		cout << endl;
    }
}

bool main_table::contains_type(string type)
{
	unsigned int code = hash_string(type);

	unsigned int hash_key = hash_number(basic_table, code);

    if (sec_tables[hash_key].new_table->m == 0)
		return false;

	unsigned int hash_sec_key = hash_number(sec_tables[hash_key].new_table, code);

	if (sec_tables[hash_key].cells[hash_sec_key] == code)
		return true;
	return false;
}

bool main_table::contains_product(string product)
{
	unsigned int code = hash_string(product);

	unsigned int hash_key = hash_number(basic_table, code);

    if (sec_tables[hash_key].new_table->m == 0)
		return false;

	unsigned int hash_sec_key = hash_number(sec_tables[hash_key].new_table, code);

	if (sec_tables[hash_key].cells[hash_sec_key] == code)
		return true;
	return false;
}

int main() {
	int choice;//number of elements
	products data;
	int i, pos;
	string type_p, product_p;

	srand (time(0));

	do {
		cout << "If file is not empty, reading data from file!" << endl;
		data.read_data_from_file();
		cout << "1)Add new data." << endl;
		cout << "2)Delete data." << endl;
		cout << "3)Use data for hashing." << endl;
		cin >> choice;
		if (choice == 1)
			data.new_data();
		if (choice == 2) {
			string type_p, product_p;
			int i, pos;
			cout << "Write a group of product :" << endl;
			cin.get();
			getline(cin, type_p);

			i = data.check_is_type(type_p);
			if (i == -1)
				continue;
			cout << "Write a name of product:" << endl;
			cin.get();
			getline(cin, product_p);

			pos = data.check_is_product(i, product_p);
			if (pos == -1)
				continue;
			data.delete_data(data.type_of_products[i]->array_of_products[pos]);
		}
		if (choice == 3)
			data.save_changing();

	} while (choice != 3);
	p = successor_prime(max_number(data.type_of_products));
	main_table T(unsigned int(data.type_of_products.size()));


	cout << "Hashing types..." << endl;

	T.Initialize_types(data.type_of_products);
	
	cout << "All products have a hash key." << endl;
	cout << "Write a group of product :" << endl;
	cin.get();
	getline(cin, type_p);
	
	if (T.contains_type(type_p)) {
		cout << "This is in table." << endl;
		int i = data.check_is_type(type_p);
		p = successor_prime(max_number(data.type_of_products[i]->array_of_products));

		main_table P(unsigned int(data.type_of_products[i]->array_of_products.size()));
		P.Initialize_products(data.type_of_products[i]->array_of_products);

		cout << "Write a name of product :" << endl;
		//cin.get();
		getline(cin, product_p);

		if (P.contains_product(product_p))
			cout << "This is in table." << endl;
		else
			cout << "No, there isn't something like this." << endl;
	}
	else
		cout << "No, there isn't something like this." << endl;

	system("pause");
	return 0;

}
