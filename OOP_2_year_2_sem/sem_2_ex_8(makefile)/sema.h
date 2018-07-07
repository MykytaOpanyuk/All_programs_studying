#include <iostream>
#include <stack>
#include <pthread.h>
#include <cstdlib>

using namespace std;

stack<int> st;
pthread_cond_t cv_high;
pthread_mutex_t lock_queue;
bool is_running;
unsigned int limit;

unsigned int all_pushes;
unsigned int all_pops;

void *push(void *param)
{
	int data;
	unsigned int pushes = 0;
	srand (time(NULL));
	while (is_running) {
		data = rand() % 100;
		pthread_mutex_lock(&lock_queue);
		st.push(data);
		if (pushes++ > limit)
			is_running = false;
		all_pushes++;
		pthread_cond_signal(&cv_high);
		pthread_mutex_unlock(&lock_queue);
	}
	pthread_mutex_lock(&lock_queue);
	cout << "Thread id = " << pthread_self() <<
			", pushes = " << pushes << std::endl;
	pthread_mutex_unlock(&lock_queue);

	return NULL;
}

void *pop(void *param)
{
	unsigned int pops = 0;
	bool prev_success = false;
	static struct timespec time_to_wait = {0, 0};
	time_to_wait.tv_sec = time(NULL) + 1;

	while (is_running || prev_success) {
		pthread_mutex_lock(&lock_queue);
		if(!st.empty()) {
			st.pop();
			prev_success = true;
			pops++;
			all_pops++;
		} else {
			prev_success = false;
			pthread_cond_timedwait(&cv_high, &lock_queue, &time_to_wait);
		}

		pthread_mutex_unlock(&lock_queue);
	}
	pthread_mutex_lock(&lock_queue);
	cout << "Thread id = " << pthread_self() <<
			", pops = " << pops << std::endl;
	pthread_mutex_unlock(&lock_queue);

	return NULL;
}

class modified_semaphore {
public :

	//limit is a count of numbers for a threads to push in stack
	modified_semaphore(unsigned int num_to_read, unsigned int num_to_write, unsigned int limit_size)
	{
		limit = limit_size;

		pthread_attr_t tattr;
		sched_param param;
		is_running = true;
		int ret;
		
		all_pushes = 0;
		all_pops = 0;
		pthread_t high_priority[num_to_read];
		pthread_t low_priority[num_to_write];

		for (unsigned int j = 0; j < num_to_read; j++) {
			// initialized with default attributes
			ret = pthread_attr_init (&tattr);
			// safe to get existing scheduling param
			ret = pthread_attr_getschedparam (&tattr, &param);
			// set the priority; others are unchanged
			param.sched_priority = sched_get_priority_max(SCHED_FIFO);
			// setting the new scheduling param
			ret = pthread_attr_setschedparam (&tattr, &param);

			pthread_create(&high_priority[j], &tattr, pop, NULL);
		}

		cout << "Preparing threads..." << endl;

		for (unsigned int j = 0; j < num_to_write; j++) {
			// initialized with default attributes
			ret = pthread_attr_init(&tattr);
			// safe to get existing scheduling param
			ret = pthread_attr_getschedparam (&tattr, &param);
			// set the priority; others are unchanged
			param.sched_priority = sched_get_priority_min(SCHED_FIFO);
			// setting the new scheduling param
			ret = pthread_attr_setschedparam (&tattr, &param);

			pthread_create(&low_priority[j], &tattr, push, NULL);
		}

		for (unsigned int j = 0; j < num_to_write; j++)
			pthread_join(low_priority[j], NULL);
		for (unsigned int j = 0; j < num_to_read; j++)
			pthread_join(high_priority[j], NULL);

	}
};

