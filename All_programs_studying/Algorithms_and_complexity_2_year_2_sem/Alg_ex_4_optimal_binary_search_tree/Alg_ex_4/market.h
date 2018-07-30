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
	double probability;
	double fact_probability;
	product_element *left;
	product_element *right;
};

bool is_empty_file(fstream &file)
{
	return (file.peek() == ifstream::traits_type::eof());
}

class market
{

public:
	unsigned int size_of_f;
	vector<product_element *> main_array_of_products;

	market();
	~market();
	void read_data_from_file();
};

market::market()
{
	cout << "If file is not empty, reading data from file!" << endl;
	read_data_from_file();
}

market::~market()
{
	main_array_of_products.clear();
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


	while (j != size_of_f + 1) {
		product_element *value = new product_element;
		getline(file, value->group, ';');
		getline(file, value->name, ';');
		file >> value->price;
		file >> value->probability;
		file >> value->fact_probability;
		file.get(c); // reading \n

		main_array_of_products.push_back(value);

		j++;
	}
	file.close();
}

market *main_data;