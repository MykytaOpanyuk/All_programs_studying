#include "test_thread.h"

#define MAX 100000
#define MIN -100000

vector<vector<int> > vector_for_sorting;

void my_swap(int *a, int *b)
{
    int c;
    c = *a;
    *a = *b;
    *b = c;
}

void quick_sort(vector<vector<int> > &arr, int index, int low, int high)
{
    int pivot = arr[index][(low + high)/2];
    int i = low;
    int j = high;

    while (i <= j) {
        while (arr[index][i] < pivot)
            i++;
        while (arr[index][j] > pivot)
            j--;
        if (i <= j) {
            my_swap(&arr[index][i], &arr[index][j]);
            i++;
            j--;
        }
    }

    if (low < j)
        quick_sort(arr, index, low, j);
    if (i < high)
        quick_sort(arr, index, i, high);
}

void work_with_thread(int index)
{
    int max = vector_for_sorting[index].size();

    quick_sort(vector_for_sorting, index, 0, (max - 1));
}

Test_Thread::Test_Thread(QObject *parent) : QObject(parent)
{}

void Test_Thread::bucket_sort(vector <int> & One, int n)
{

    vector_for_sorting.resize(n);
    int minEl = MAX;
    int maxEl = MIN;
    for (int i = 0; i < (int)One.size(); i++) {
        if (One[i] > maxEl)
            maxEl = One[i];
        if (One[i] < minEl)
            minEl = One[i];
    }
    int divi = (maxEl - minEl) / n;

    for (int i = 0; i < (int)One.size(); i++) {
        if (One[i] >= minEl + divi*n) {
            vector_for_sorting[n - 1].push_back(One[i]);
            continue;
        }
        if (One[i] == minEl) {
            vector_for_sorting[0].push_back(One[i]);
            continue;
        }
        int num = (One[i] - minEl) / divi;
        vector_for_sorting[num].push_back(One[i]);
    }

    for (int i = 0; i < n; i++) {
        int max = vector_for_sorting[i].size();
        quick_sort(vector_for_sorting, i, 0, (max - 1));
    }

    One.clear();
    for (int i = 0; i < n; i++)
        One.insert(One.end(), vector_for_sorting[i].begin(), vector_for_sorting[i].end());

    vector_for_sorting.clear();
}

void Test_Thread::bucket_sort_thread(vector <int> & One, int n)
{

    std::thread thread_id[n];

    for (int i = 0; i < n; i++) {
        thread_id[i] = std::thread(work_with_thread, i);
    }

    vector_for_sorting.resize(n);


    int minEl = MAX;
    int maxEl = MIN;
    for (int i = 0; i < (int)One.size(); i++) {
        if (One[i] > maxEl)
            maxEl = One[i];
        if (One[i] < minEl)
            minEl = One[i];
    }
    int divi = (maxEl - minEl) / n;

    for (int i = 0; i < (int)One.size(); i++) {
        if (One[i] >= minEl + divi*n) {
            vector_for_sorting[n - 1].push_back(One[i]);
            continue;
        }
        if (One[i] == minEl) {
            vector_for_sorting[0].push_back(One[i]);
            continue;
        }
        int num = (One[i] - minEl) / divi;
        vector_for_sorting[num].push_back(One[i]);
    }

    for (int i = 0; i < n; i++)
           thread_id[i].join();

    One.clear();
    for (int i = 0; i < n; i++)
        One.insert(One.end(), vector_for_sorting[i].begin(), vector_for_sorting[i].end());

    vector_for_sorting.clear();
}

void Test_Thread::toUpper()
{
    srand(time(NULL));
    vector<int> array1, array2;
    int n = 3, l = 10000;

    for (int i = 0; i < l; i++) {
        int a = rand() % 1000;
        array1.push_back(a);
        array2.push_back(a);

    }
    clock_t time = clock();
    bucket_sort_thread(array1, n);
    time = clock() - time;

    cout << endl << "Time of executing with threads: " << (float)time / CLOCKS_PER_SEC << endl;

    clock_t new_time = clock();
    bucket_sort(array2, n);
    new_time = clock() - new_time;

    //for (unsigned int i = 0; i < array2.size(); i++)
    //    cout << array2[i] << " ";

    cout << endl << "Time of executing without threads: " << (float)new_time / CLOCKS_PER_SEC << endl;
    QCOMPARE(array1[1], array2[1]);
}

QTEST_MAIN(Test_Thread)
//#include "test_thread.moc"
