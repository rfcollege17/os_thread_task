#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include "../../header/teste.h"
#include "../../lib/vet/vet.h"

void criaThreads();
char *threadName(int threadNumber);
void retiraPares();
void retiraMultiplosDeCinco();

int *randomNumbers;
int tam;
sem_t semaforo;

void main()
{
  sem_init(&semaforo, 0, 1);

  printf("Por favor, digite o tamanho do vetor aleatório: ");
  scanf("%d", &tam);

  randomNumbers = Vet_init(tam);
  Vet_put_random(randomNumbers, tam);

  int *original = Vet_clone(randomNumbers, tam);
  int tamOriginal = tam;

  printf("\e[33mEsse é o vetor original\e[m\n");
  Vet_print(randomNumbers, tam);

  criaThreads();

  printf("\n\e[33mEsse é o vetor modificado\e[m\n");
  Vet_print(randomNumbers, tam);

  if (testaRemocaoParalela(original, tamOriginal, randomNumbers, tam))
    printf("\e[5;32mPassou nos testes\e[m\n");
  else
    printf("\e[31mFalho nos testes...\e[m\n");

  free(randomNumbers);
  pthread_exit(NULL);
}

// gera as 2 threads dizendo oque cada uma vai fazer
void criaThreads()
{
  int errorCode;
  void *callback;
  pthread_t threads[2];
  for (int i = 0; i < 2; i++)
  {
    callback = (i % 2 == 0) ? retiraPares : retiraMultiplosDeCinco;
    pthread_t currentThread = 0L;
    errorCode = pthread_create(&currentThread, NULL, callback, NULL);

    if (errorCode)
    {
      printf("\e[31mErro %d na criação da %s\e[m\n", errorCode, threadName(i));
      exit(1);
    }

    threads[i] = currentThread;
  }

  for (int i = 0; i < 2; i++)
    pthread_join(threads[i], NULL);
}

// retira numeros pares de trás pra frente do vetor
void retiraPares()
{
  for (int i = tam - 1; i >= 0; i--)
  {
    sem_wait(&semaforo);
    if (!(randomNumbers[i] % 2))
    {
      printf("thread dos pares remove: %i\n", randomNumbers[i]);
      Vet_remove(randomNumbers, &tam, i);
    }
    sem_post(&semaforo);
  }
  pthread_exit(NULL);
}

// retira numeros multiplos de 5 de trás pra frente do vetor
void retiraMultiplosDeCinco()
{
  for (int i = tam - 1; i >= 0; i--)
  {
    sem_wait(&semaforo);
    if (!(randomNumbers[i] % 5))
    {
      printf("thread dos multiplos de 5 remove %i\n", randomNumbers[i]);
      Vet_remove(randomNumbers, &tam, i);
    }
    sem_post(&semaforo);
  }
  pthread_exit(NULL);
}

// diz qual thread é referenciada pelo numero passado
char *threadName(int threadNumber)
{
  char *name = (char *)malloc(sizeof(char) * 30);
  name = (threadNumber % 2 == 0) ? "thread dos pares" : "thread dos múltiplos de 5";
  return name;
}