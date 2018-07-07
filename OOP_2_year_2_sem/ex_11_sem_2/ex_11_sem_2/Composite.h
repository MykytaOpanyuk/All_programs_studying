#pragma once
#include "calculator.h"
#include <string>
#include <stdexcept>

using namespace std;

int Priority(char a)
{
	if (a == '(')
		return 0;
	if (a == '^')
		return 40;
	if (a == '/')
		return 30;
	if (a == '*')
		return 30;
	if (a == '%')
		return 30;
	if (a == '+')
		return 20;
	if (a == '-')
		return 20;

	return 0;
}

class Expression {
public:
	virtual void calculate(calculator *) = 0;
};

class Constant : public Expression {

public:

	Constant(): Expression() {};

	void calculate(calculator *new_calc) override
	{	
		while ((new_calc->input_string[new_calc->i] >= '0' && new_calc->input_string[new_calc->i] <= '9') 
			|| new_calc->input_string[new_calc->i] == '.') {
			new_calc->output_string[new_calc->j++] = new_calc->input_string[new_calc->i];
			new_calc->i++;
			if (!(new_calc->input_string[new_calc->i] >= '0' && new_calc->input_string[new_calc->i] <= '9'))
				if (new_calc->input_string[new_calc->i] != '.') {
					new_calc->output_string.insert(new_calc->j, " ");
					new_calc->j++;
				}
		}
	}

};

class closed_bracket : public Expression {

public:

	closed_bracket() {};

	void calculate(calculator *new_calc) override
	{
		char d = '\0';

		while ((new_calc->stack_timetable->elem[new_calc->stack_timetable->head - 1]) != '(') {
			new_calc->DEL_EL(d, new_calc->stack_timetable);
			new_calc->output_string[new_calc->j++] = d;
			new_calc->output_string.insert(new_calc->j, " ");
			new_calc->j++;
		}
		new_calc->DEL_EL(d, new_calc->stack_timetable);
	}

};

class opened_bracket : public Expression {
public:

	opened_bracket() {};

	void calculate(calculator *new_calc) override
	{
		new_calc->ADD_EL(new_calc->input_string[new_calc->i], new_calc->stack_timetable);
	}

};

class operation: public Expression {
public:

	operation() {};

	void calculate(calculator *new_calc) override
	{
		char d;

		if (new_calc->stack_timetable->head == 0)
			new_calc->ADD_EL(new_calc->input_string[new_calc->i], new_calc->stack_timetable);
		else {
			while ((new_calc->stack_timetable->head != -1) &&
				(Priority(new_calc->stack_timetable->elem[new_calc->stack_timetable->head - 1])
					>= Priority(new_calc->input_string[new_calc->i]))) {
				new_calc->DEL_EL(d, new_calc->stack_timetable);
				new_calc->output_string[new_calc->j++] = d;
				new_calc->output_string.insert(new_calc->j, " ");
				new_calc->j++;
			}
			new_calc->ADD_EL(new_calc->input_string[new_calc->i], new_calc->stack_timetable);
		}
	}

};

class Unar : public Expression {
public:
	Unar() {};

	void calculate(calculator *new_calc) {

		if ((new_calc->input_string[new_calc->i - 1] == '(') || (new_calc->i == 0)) {
			new_calc->output_string[new_calc->j++] = '0';
			new_calc->output_string.insert(new_calc->j, " ");
			new_calc->j++;
			new_calc->ADD_EL(new_calc->input_string[new_calc->i], new_calc->stack_timetable);
		}
	}
};
