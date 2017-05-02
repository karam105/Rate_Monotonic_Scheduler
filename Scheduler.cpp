#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <semaphore.h>

using namespace std;

sem_t semaphore;
sem_t mutex1;
sem_t mutex2;
sem_t mutex3;
sem_t mutex4;

// initialze variables
int i = 0;
int overrun1 = 0;
int overrun2 = 0;
int overrun3 = 0;
int overrun4 = 0;
int workingArray[100][100];

int doWork();
void* p1(void *param);
void* p2(void *param);
void* p3(void *param);
void* p4(void *param);


int main(int argc, char const *argv[])
{
	sem_init(&mutex1, 0, 0);
	sem_init(&mutex2, 0, 0);
	sem_init(&mutex3, 0, 0);
	sem_init(&mutex4, 0, 0);

// initialze all threads
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread4;

// actually create all threads
	pthread_create(&thread1, NULL, p1, NULL);
	pthread_create(&thread2, NULL, p2, NULL);
	pthread_create(&thread3, NULL, p3, NULL);
	pthread_create(&thread4, NULL, p4, NULL);

	while (i < 160)
	{
		if (i == 0) // initial case.  at time 0 schedule all threads
		{
			sem_post(&mutex1);
			sem_post(&mutex2);
			sem_post(&mutex3);
			sem_post(&mutex4);
		}

		else if (i % 16 == 0) // last case which happens every 16 units which schedules all threads again
		{
			sem_post(&mutex1);
			sem_post(&mutex2);
			sem_post(&mutex3);
			sem_post(&mutex4);
		}

		else if (i % 4 == 0) // case that happens every 4 units of time
		{
			sem_post(&mutex1);
			sem_post(&mutex2);
			sem_post(&mutex3);
		}

		else if (i % 2 == 0) // case that happens every other unit of time
		{
			sem_post(&mutex1);
			sem_post(&mutex2);
		}

		else if (i % 1 == 0) // case that happens every unit of time
		{
			sem_post(&mutex1);
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

int doWork()
{
	int lousyArray[10][10];
	int product = 1;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			lousyArray[i][j] = 1;
		}
	}
	for (int k = 0; k < 1; k++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				product *= lousyArray[i][j];
			}
		}
	}
	return 1;
}


void* p1(void *param)
{
	bool thread1FinishFlag = false;
	while(1)
	{
		sem_wait(&mutex1);
		thread1FinishFlag = false;
		for (int i = 0; i < 1; i++)
		{
			doWork();
		}
		//increment counter here
		thread1FinishFlag = true;
	}
}
void* p2(void *param)
{
	bool thread2FinishFlag = false;
	while(1)
	{
		sem_wait(&mutex2);
		thread2FinishFlag = false;
		for (int i = 0; i < 2; i++)
		{
			doWork();
		}
		//increment counter here
		thread2FinishFlag = true;
	}
}
void* p3(void *param)
{
	bool thread3FinishFlag = false;
	while(1)
	{
		sem_wait(&mutex3);
		thread3FinishFlag = false;
		for (int i = 0; i < 4; i++)
		{
			doWork();
		}
		//increment counter here
		thread3FinishFlag = true;
	}
}
void* p4(void *param)
{
	bool thread4FinishFlag = false;
	while(1)
	{
		sem_wait(&mutex4);
		thread4FinishFlag = false;
		for (int i = 0; i < 16; i++)
		{
			doWork();
		}
		//increment counter here
		thread4FinishFlag = true;
	}
}

void nsleep()
{
	struct timespec delay;

	delay.tv_sec = 0;
	delay.tv_sec = 100000000L;
	nanosleep(&delay, NULL);
}
