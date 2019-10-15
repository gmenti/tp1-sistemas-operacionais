/**
 * Nome: Giuseppe Menti
 * Premissas:
 * - Não utilizar mecanismos de exclusão mútuas já presentes na biblioteca pthreads; 
 * - Utilizar algoritmo de Peterson que implementa a exclusão mútua por software;
 * - Utilizar biblioteca pthreads e implementar o problema dos produtores/consumidores (sincronização com semáforos);
 * - Proteger regiões críticas com o mutex;
 * - Número de threads da aplicação deve ser parametrizável (macro ou args);
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define THREADS_COUNT 5
#define MAX_PRODUCTS 10

sem_t producers, consumers;
sem_t mutex;

int products[MAX_PRODUCTS];
int producedAmount = 0;
int lastProduct = 0;
int beforeFirstProduct = 0;

void *producer(void *arg)
{
  int threadId = (int) arg;
  while (1) {
    printf("[PRODUCER][THREAD_%d] Waiting consumers\n", threadId);
    sem_wait(&consumers);
    sem_wait(&mutex);
    lastProduct = (lastProduct + 1) / MAX_PRODUCTS;
    products[lastProduct] = 1;
    producedAmount++;
    printf("[PRODUCER][THREAD_%d] Produced in position %d\n", threadId, lastProduct);
    sem_post(&mutex);
    sem_post(&producers);
  }
}

void *consumer(void *arg)
{
  int threadId = (int) arg;
  while (1) {
    sem_wait(&mutex);
    if (producedAmount > 0) {
      beforeFirstProduct = (beforeFirstProduct + 1) / MAX_PRODUCTS;
      products[beforeFirstProduct] = 0;
      producedAmount--;
      printf("[CONSUMER][THREAD_%d] Consumed in position %d\n", threadId, producedAmount);
      sem_post(&consumers);
      sem_post(&mutex);
      sem_wait(&producers);
    } else {
      sem_post(&mutex);
    }
  }
}

int main()
{
  sem_init(&producers, 0, 0);
  sem_init(&consumers, 0, 0);
  sem_init(&mutex, 0, 1);

  pthread_t producerThreads[THREADS_COUNT], consumerThreads[THREADS_COUNT];

  for (int i = 0; i < THREADS_COUNT; i++)
  {
		pthread_create(&producerThreads[i], NULL, producer, (void *) i);
  }

  for (int i = 0; i < THREADS_COUNT; i++)
  {
		pthread_create(&consumerThreads[i], NULL, consumer, (void *) i);
  }

  pthread_exit(NULL);	
  return 0;
}
