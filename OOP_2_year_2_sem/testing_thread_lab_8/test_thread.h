#ifndef TEST_THREAD_H
#define TEST_THREAD_H

#include <QObject>
#include <QtTest/QtTest>
#include <QCoreApplication>
#include <iostream>
#include <vector>
#include <thread>
#include <ctime>
#include <cstdlib>

using namespace std;

class Test_Thread : public QObject
{
    Q_OBJECT
public:

    void bucket_sort(vector <int> &One, int n);
    void bucket_sort_thread(vector <int> & One, int n);

    explicit Test_Thread(QObject *parent = nullptr);
private slots:
    void toUpper();
};

#endif // TEST_THREAD_H
