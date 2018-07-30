#include "strategy.h"

void stable::sort(vector<int> &to_sort)
{
    unsigned int j, k = to_sort.size() - 1;
    unsigned int lb = 1, ub = to_sort.size() - 1;

    do {
        for (j = ub; j > 0; j--)
            if (to_sort[j - 1] > to_sort[j]) {
                swap(to_sort[j - 1], to_sort[j]);
                k = j;
            }

        lb = k + 1;

        for (j = 1; j <= ub; j++) {
            if (to_sort[j - 1] > to_sort[j]) {
                swap(to_sort[j - 1], to_sort[j]);
                k = j;
            }
        }

        ub = k - 1;
    } while (lb < ub);
}

void unstable::sort(vector<int> &to_sort)
{
    for (unsigned int i = 0; i < to_sort.size() - 1; i++) {
        unsigned int min = i;
        for (unsigned int j = i + 1; j < to_sort.size(); j++)
            if (to_sort[j] < to_sort[min])
                min = j;
        if (min != i)
            swap(to_sort[i], to_sort[min]);
    }
}

Init::Init(QObject *parent) : QObject(parent)
{
    srand(time(NULL));
    int l = 1000;

    int chose_strategy_for_1 = 0;
    int chose_strategy_for_2 = 1;

    for (int i = 0; i < l; i++) {
        int a = rand() % 1000;
        array1.push_back(a);
        array2.push_back(a);
    }
    if (chose_strategy_for_1 == 0)
        set_strategy(new stable);
    else
        set_strategy(new unstable);

    this->sort_strategy(array1);

    if (chose_strategy_for_2 == 0)
        set_strategy(new stable);
    else
        set_strategy(new unstable);

    this->sort_strategy(array2);

    QCOMPARE(array1[1], array2[1]);
    QCOMPARE(array1[4], array2[4]);
}


void Init::sort_strategy(vector<int> &to_sort)
{
    strategy->sort(to_sort);
}

void array_init::set_strategy(Strategy* strategy)
{
    this->strategy = strategy;
}
QTEST_MAIN(Init)
