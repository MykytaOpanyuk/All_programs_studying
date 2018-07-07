#pragma once

#include "stdafx.h"
#include <string>
#include <iostream>
#include <math.h>
#include "Composite.h"
#include "calculator.h"

#define NMAX 20

using namespace std;

calculator::calculator(string init)
{

	stack_timetable = new stack_char;
	result = new stack_result;

	result->head = 0;
	stack_timetable->head = 0;

	for (int k = 0; k < NMAX; k++)
		stack_timetable->elem[k] = '\0';

	bool PEREV = true;

	//cout << "Write your expression: " << endl;
	//cin >> input_string;
	i = 0;
	j = 0;
	output_string = input_string = init;

	while (i < input_string.size() || (PEREV == false)) {
		if ((input_string[i] >= '0') && (input_string[i] <= '9')) {
			Constant a;
			a.calculate(this);
		}
		if (input_string[i] == ')') {
			closed_bracket a;
			a.calculate(this);
		}
		if (input_string[i] == '(') {
			opened_bracket a;
			a.calculate(this);
		}
		if (input_string[i] == '/' || input_string[i] == '*' || input_string[i] == '%' || input_string[i] == '^'
			|| input_string[i] == '+' || input_string[i] == '-') {
			operation a;
			a.calculate(this);
		}
		if (input_string[i] == '+' || input_string[i] == '-') {
			Unar a;
			a.calculate(this);
		}
		i++;
	}
	char d;

	while (stack_timetable->head != 0) {
		DEL_EL(d, stack_timetable);
		output_string[j++] = d;
	}

	RES(output_string, result, j);
	cout << endl;

}
calculator::~calculator()
{
	delete result;
	delete stack_timetable;
	input_string.empty();
	output_string.empty();
}

void calculator::ADD_EL(char a, stack_char *stack)
{
	stack->elem[stack->head] = a;
	stack->head++;
}

void calculator::DEL_EL(char &d, stack_char *stack)
{
	d = stack->elem[stack->head - 1];
	stack->head--;
}

void calculator::RES(string output_string, stack_result *STRES, int size)
{
	int i = 0;
	int j = 0;
	string small_string;
	double ch;
	while ((i < size)) {
		while ((output_string[i] >= '0' && output_string[i] <= '9') || output_string[i] == '.') {
			small_string[j++] = output_string[i];
			i++;
		}
		for (int k = 0; k < j; k++)
			if (small_string[k] != '\0') {
				ch = atof(small_string.c_str());
				ADD_EL_RES(ch, STRES);
				break;
			}
		for (int k = 0; k < j; k++)
			small_string[k] = NULL;
		j = 0;
		if (output_string[i] == '/') {
			STRES->elem[STRES->head - 2] = (STRES->elem[STRES->head - 2] / STRES->elem[STRES->head - 1]);
			DEL_EL_RES(STRES);
		}
		if (output_string[i] == '*') {
			STRES->elem[STRES->head - 2] = (STRES->elem[STRES->head - 2] * STRES->elem[STRES->head - 1]);
			DEL_EL_RES(STRES);
		}
		if (output_string[i] == '%') {
			STRES->elem[STRES->head - 2] = (double)((int)((round)(STRES->elem[STRES->head - 2])) %
				(int)((round)(STRES->elem[STRES->head - 1])));
			DEL_EL_RES(STRES);
		}
		if (output_string[i] == '^') {
			STRES->elem[STRES->head - 2] = pow(STRES->elem[STRES->head - 2], STRES->elem[STRES->head - 1]);
			DEL_EL_RES(STRES);
		}
		if (output_string[i] == '-') {
			STRES->elem[STRES->head - 2] = (STRES->elem[STRES->head - 2] - STRES->elem[STRES->head - 1]);
			DEL_EL_RES(STRES);
		}
		if (output_string[i] == '+') {
			STRES->elem[STRES->head - 2] = (STRES->elem[STRES->head - 2] + STRES->elem[STRES->head - 1]);
			DEL_EL_RES(STRES);
		}
		i++;
	}
	cout << "Your result is: " << STRES->elem[0];
}

void calculator::ADD_EL_RES(double a, stack_result *stack_result)
{
	stack_result->elem[stack_result->head] = a;
	stack_result->head++;
}

void calculator::DEL_EL_RES(stack_result *stack_result)
{
	stack_result->head--;
}