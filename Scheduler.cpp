#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <semaphore.h>

using namespace std;

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
int loop1 = 0;
int loop2 = 0;
int loop3 = 0;
int loop4 = 0;

bool thread1FinishFlag = false;
bool thread2FinishFlag = false;
bool thread3FinishFlag = false;
bool thread4FinishFlag = false;

pthread_attr_t mattr;
pthread_attr_t tattr;

int mainpriority = 1;
int threadpriority = 2;

int doWork();
void nsleep();
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
		nsleep();
		if (i == 0) // initial case.  at time 0 schedule all threads
		{
			sem_post(&mutex1);
			sem_post(&mutex2);
			sem_post(&mutex3);
			sem_post(&mutex4);
		}

		else if (i % 16 == 0) // last case which happens every 16 units which schedules all threads again
		{
			if (thread1FinishFlag == false){overrun1++;}
			if (thread2FinishFlag == false){overrun2++;}
			if (thread3FinishFlag == false){overrun3++;}
			if (thread4FinishFlag == false){overrun4++;}
			sem_post(&mutex1);
			sem_post(&mutex2);
			sem_post(&mutex3);
			sem_post(&mutex4);
		}

		else if (i % 4 == 0) // case that happens every 4 units of time
		{
			if (thread1FinishFlag == false){overrun1++;}
			if (thread2FinishFlag == false){overrun2++;}
			if (thread3FinishFlag == false){overrun3++;}
			sem_post(&mutex1);
			sem_post(&mutex2);
			sem_post(&mutex3);
		}

		else if (i % 2 == 0) // case that happens every other unit of time
		{
			if (thread1FinishFlag == false){overrun1++;}
			if (thread2FinishFlag == false){overrun2++;}
			sem_post(&mutex1);
			sem_post(&mutex2);
		}

		else if (i % 1 == 0) // case that happens every unit of time
		{
			if (thread1FinishFlag == false){overrun1++;}
			sem_post(&mutex1);
		}
		i++; // increment i to go through the loop again
	}

	cout << "Total runs: " << i << endl;
	cout << "Thread 1 overruns: " << overrun1 << endl;
	cout << "Thread 2 overruns: " << overrun2 << endl;
	cout << "Thread 3 overruns: " << overrun3 << endl;
	cout << "Thread 4 overruns: " << overrun4 << endl;

	cout << "Total thread1 runs: " << loop1 << endl;
	cout << "Total thread2 runs: " << loop2 << endl;
	cout << "Total thread3 runs: " << loop3 << endl;
	cout << "Total thread4 runs: " << loop4 << endl;

	sleep(1);
	pthread_cancel(thread1);
	pthread_cancel(thread2);
	pthread_cancel(thread3);
	pthread_cancel(thread4);
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
	thread1FinishFlag = false;
	while(1)
	{
		sem_wait(&mutex1);
		thread1FinishFlag = false;
		for (int i = 0; i < 1; i++)
		{
			doWork();
		}
		loop1++;
		thread1FinishFlag = true;
	}
}

void* p2(void *param)
{
	thread2FinishFlag = false;
	while(1)
	{
		sem_wait(&mutex2);
		thread2FinishFlag = false;
		for (int i = 0; i < 2; i++)
		{
			doWork();
		}
		loop2++;
		thread2FinishFlag = true;
	}
}

void* p3(void *param)
{
	thread3FinishFlag = false;
	while(1)
	{
		sem_wait(&mutex3);
		thread3FinishFlag = false;
		for (int i = 0; i < 4; i++)
		{
			doWork();
		}
		loop3++;
		thread3FinishFlag = true;
	}
}

void* p4(void *param)
{
	thread4FinishFlag = false;
	while(1)
	{
		sem_wait(&mutex4);
		thread4FinishFlag = false;
		for (int i = 0; i < 16; i++)
		{
			doWork();
		}
		loop4++;
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
