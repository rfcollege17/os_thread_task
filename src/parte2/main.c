#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include "../../header/parte2/teste.h"
#include "../../lib/vet/vet.h"

void criaThreads();
char *threadName(int threadNumber);
void retiraPares();
void retiraMultiplosDeCinco();

Vet *randomNumbers;
sem_t semaforo;

void main(){
  sem_init(&semaforo, 0, 1);

  printf("Por favor, digite o tamanho do vetor aleatório: ");
  int tam;
  scanf("%d", &tam);

  randomNumbers = Vet_init(tam);
  randomNumbers->size = tam;
  Vet_put_random(randomNumbers);

  Vet *original = Vet_clone(randomNumbers);
  
  printf("\e[33mEsse é o vetor original\e[m\n");
  Vet_print(randomNumbers);

  criaThreads();

  printf("\n\e[33mEsse é o vetor modificado\e[m\n");
  Vet_print(randomNumbers);

  if (testaParte2(original, randomNumbers))
    printf("\e[5;32mPassou nos testes\e[m\n");
  else
    printf("\e[31mFalho nos testes...\e[m\n");

  Vet_free(randomNumbers);
  pthread_exit(NULL);
}

// gera as 2 threads dizendo oque cada uma vai fazer
void criaThreads(){
  int errorCode;
  void * callback;
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
void retiraPares(){
  for (int i = randomNumbers->size - 1; i >= 0; i--)
  {
    sem_wait(&semaforo);
    if(!(randomNumbers->data[i]%2)){
      printf("thread dos pares remove: %i\n", randomNumbers->data[i]);
      Vet_remove(randomNumbers, i);
    }
    sem_post(&semaforo);
  }
  pthread_exit(NULL);
}

// retira numeros multiplos de 5 de trás pra frente do vetor  
void retiraMultiplosDeCinco(){
  for (int i = randomNumbers->size - 1; i >= 0; i--)
  {
    sem_wait(&semaforo);
    if(!(randomNumbers->data[i] % 5)){
      printf("thread dos multiplos de 5 remove %i\n", randomNumbers->data[i]);
      Vet_remove(randomNumbers, i);
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