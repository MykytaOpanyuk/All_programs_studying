#ifndef STRATEGY_H
#define STRATEGY_H

#include <QObject>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <QtTest/QtTest>
#include <ctime>

using namespace std;

class Strategy
{
public:
    virtual void sort(vector<int> &to_sort) = 0;
};

class stable : public Strategy
{
public:
    void sort(vector<int> &to_sort);
};

class unstable : public Strategy
{
public:
    void sort(vector<int> &to_sort);
};

class array_init
{
protected:
    Strategy * strategy;

public:
    virtual void sort_strategy(vector<int> &to_sort) = 0;
    virtual void set_strategy(Strategy*);
};

class Init : public QObject, public array_init {

public:
    vector<int> array1, array2;

     explicit Init(QObject *parent = nullptr);
    void sort_strategy(vector<int> &to_sort);

};

#endif // STRATEGY_H
