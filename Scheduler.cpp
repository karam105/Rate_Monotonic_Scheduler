#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <semaphore.h>

using namespace std;

sem_t semaphore;

// initialze variables
int i = 0;
int overrun1 = 0;
int overrun2 = 0;
int overrun3 = 0;
int overrun4 = 0;
int workingArray[100][100];

void* doWork(void*);


int main(int argc, char const *argv[])
{
	sem_init(&semaphore, 0, 1);

// initialze all threads
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread4;

// actually create all threads
	pthread_create(&thread1, NULL, doWork, NULL);
	pthread_create(&thread2, NULL, doWork, NULL);
	pthread_create(&thread3, NULL, doWork, NULL);
	pthread_create(&thread4, NULL, doWork, NULL);

	while (i < 160)
	{
		if (i == 0) // initial case.  at time 0 schedule all threads
		{
			sem_signal(thread1);
			sem_signal(thread2);
			sem_signal(thread3);
			sem_signal(thread4);
		}

		else if (i % 16 == 0) // last case which happens every 16 units which schedules all threads again
		{
			sem_signal(thread1);
			sem_signal(thread2);
			sem_signal(thread3);
			sem_signal(thread4);
		}

		else if (i % 4 == 0) // case that happens every 4 units of time
		{
			sem_signal(thread1);
			sem_signal(thread2);
			sem_signal(thread3);
		}

		else if (i % 2 == 0) // case that happens every other unit of time
		{
			sem_signal(thread1);
			sem_signal(thread2);
		}

		else if (i % 1 == 0) // case that happens every unit of time
		{
			sem_signal(thread1);
		}
		i++; // increment i to go through the loop again
	}

// join all threads back to the main one
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);

	return 0;
}

// doWork function

void* doWork(void*)
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			workingArray[i][j] = 1;
		}
	}
}

/* possible timer in microseconds
If you are using c++11 or later you could use std::chrono::high_resolution_clock.

auto start = std::chrono::high_resolution_clock::now();
...
auto elapsed = std::chrono::high_resolution_clock::now() - start;

long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();*/
