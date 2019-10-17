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
#include <stdbool.h> 

#define CONSUMER_COUNT 5
#define PRODUCER_COUNT 5

sem_t producers, consumers, managing;

int producedAmount = 0;
int level[CONSUMER_COUNT + PRODUCER_COUNT];
int lastToEnter[CONSUMER_COUNT + PRODUCER_COUNT - 1];

void lock(int id)
{
  for (int i = 0; i < CONSUMER_COUNT + PRODUCER_COUNT - 1; i++) 
  {
    level[id] = i;
    lastToEnter[i] = id;
    while (true) 
    {
      bool ok1 = lastToEnter[i] == id;
      bool ok2 = false;
      for (int j = 0; j < CONSUMER_COUNT + PRODUCER_COUNT - 1; j++) 
      {
        if (i == j) 
        {
          continue;
        }
        if (level[j] > i) 
        {
          ok2 = true;
        }
      }
      if (ok1 && ok2) 
      {
        continue;
      }
      break;
    }
  }
} 

void unlock(int id)
{
  level[id] = -1;
}

void *producer(void *arg)
{
  int threadId = (int) arg;
  while (1) {
    sem_wait(&consumers);
    lock(threadId);
    producedAmount++;
    printf("[PRODUCER][THREAD_%d] Produced new product\n", threadId);
    unlock(threadId);
    sem_post(&producers);
  }
}

void *consumer(void *arg)
{
  int threadId = (int) arg;
  while (1) {
    sem_post(&consumers);
    sem_wait(&producers);
    lock(threadId);
    producedAmount--;
    printf("[CONSUMER][THREAD_%d] Consumed product\n", threadId);
    unlock(threadId);
  }
}

int main()
{
  for (int i = 0; i < CONSUMER_COUNT + PRODUCER_COUNT; i++)
  {
    level[i] = 0;
  }

  sem_init(&producers, 0, 0);
  sem_init(&consumers, 0, 0);

  pthread_t producerThreads[CONSUMER_COUNT + PRODUCER_COUNT], consumerThreads[CONSUMER_COUNT + PRODUCER_COUNT];

  for (int i = 0; i < PRODUCER_COUNT; i++)
  {
		pthread_create(&producerThreads[i], NULL, producer, (void *) i);
  }

  for (int i = PRODUCER_COUNT; i < CONSUMER_COUNT + PRODUCER_COUNT; i++)
  {
		pthread_create(&consumerThreads[i], NULL, consumer, (void *) i);
  }

  pthread_exit(NULL);	

  return 0;
}
