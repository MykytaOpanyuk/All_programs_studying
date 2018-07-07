#pragma once

#include <string>
#include <iostream>

#define NMAX 20

using namespace std;

struct stack_char {
	char elem[NMAX];
	int head;
};

struct stack_result {
	double elem[NMAX];
	int head;
};

class calculator {

public:

	string input_string, output_string;
	stack_char *stack_timetable;
	stack_result *result;
	int i, j;

	calculator(string init);
	~calculator();

	void ADD_EL(char a, stack_char *stack);

	void DEL_EL(char &d, stack_char *stack);

	void RES(string output_string, stack_result *STRES, int size);

	void ADD_EL_RES(double a, stack_result *stack_result);

	void DEL_EL_RES(stack_result *stack_result);

};
