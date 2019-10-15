#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N_PHILOSOPHERS	5
#define LEFT		(i + N_PHILOSOPHERS-1) % N_PHILOSOPHERS
#define RIGHT		(i + 1) % N_PHILOSOPHERS
#define THINKING	0
#define HUNGRY		1
#define EATING		2

#define DUMMY_LOOPS	10000000

int state[N_PHILOSOPHERS];
sem_t mutex;
sem_t s[N_PHILOSOPHERS];

void test(int i){
	if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
		state[i] = EATING;
		sem_post(&s[i]);
	}
}

void take_forks(int i){
	sem_wait(&mutex);
	state[i] = HUNGRY;
	test(i);
	sem_post(&mutex);
	sem_wait(&s[i]);
}

void put_forks(int i){
	sem_wait(&mutex);
	state[i] = THINKING;
	test(LEFT);
	test(RIGHT);
	sem_post(&mutex);
}

void *philosopher(void *arg){
	long int i, j;

	i = (long int)arg;
	while(1){
		printf("thread %d thinking...\n", i);
		for(j = 0; j < DUMMY_LOOPS; j++);
		printf("thread %d hungry...\n", i);
		take_forks(i);
		printf("thread %d eating...\n", i);
		for(j = 0; j < DUMMY_LOOPS; j++);
		put_forks(i);
	}
}

int main(void){
	long int i;
	pthread_t tphilosophers[N_PHILOSOPHERS];
	
	sem_init(&mutex, 0, 1);
	for(i = 0; i < N_PHILOSOPHERS; i++)
		sem_init(&s[i], 0, 1);

	for(i = 0; i < N_PHILOSOPHERS; i++)
		pthread_create(&tphilosophers[i], NULL, philosopher, (void *)i);

	pthread_exit(NULL);	
	return(0);
}

