#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include "../../header/parte2/teste.h"

int tamanhoVetor = 1000;


void printaVetor();
void inicializaVetor(int * vet, int tamanho);
void criaThreads(int tamanho);
void retiraPares();
void remFromVet(int *vet, int posBuraco);
void retiraMultiplosDeCinco();
char *threadName(int threadNumber);

int *randomNumbers;
sem_t *semaforo;

void main(){
  semaforo = (sem_t *) malloc(sizeof(sem_t));
  randomNumbers = (int *) malloc(sizeof(int) * tamanhoVetor);

  sem_init(semaforo, 0, 1);
  inicializaVetor(randomNumbers, tamanhoVetor);
  int original[tamanhoVetor];
  int tamOriginal = tamanhoVetor;

  for (int i = 0; i < tamanhoVetor; i++)
  {
    original[i] = randomNumbers[i];
  }
  
  printf("\e[33mEsse é o vetor original\e[m\n");
  printaVetor();

  criaThreads(tamanhoVetor);

  printf("\n\e[33mEsse é o vetor modificado\e[m\n");
  printaVetor();

  if (testaParte2(original, tamOriginal, randomNumbers, tamanhoVetor))
    printf("\e[5;32mPassou nos testes\e[m\n");
  else
    printf("\e[31mFalho nos testes...\e[m\n");

  free(randomNumbers);
  free(semaforo);
  pthread_exit(NULL);
}

// printa o vetor no estado atual
void printaVetor(){
  printf("\n");
  for (int i = 0; i < tamanhoVetor; i++)
  {
    printf("%i,",randomNumbers[i]);
  }
  printf("\n\n");
}

// inicializa elementos do vetor com numeros aleatorios entre 1 e 100
void inicializaVetor(int * vet, int tamanho){
  for (int i = 0; i < tamanho; i++)
  {
    int randomNum = rand() % 100;
    vet[i] = randomNum;
  }
}

// gera as 2 threads dizendo oque cada uma vai fazer
void criaThreads(int tamanho){
  int errorCode;
  void * callback;
  pthread_t threads[2];
  for (int i = 0; i < 2; i++)
  {
    callback = (i % 2 == 0) ? retiraPares : retiraMultiplosDeCinco;
    pthread_t currentThread = 0L;
    errorCode = pthread_create(&currentThread, NULL, callback, threadName(i));

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
  for (int i = tamanhoVetor - 1; i >= 0; i--)
  {
    sem_wait(semaforo);
    if(!(randomNumbers[i]%2)){
      printf("thread dos pares remove: %i\n", randomNumbers[i]);
      remFromVet(randomNumbers, i);
    }
    sem_post(semaforo);
  }
  pthread_exit(NULL);
}

// retira numeros multiplos de 5 de trás pra frente do vetor  
void retiraMultiplosDeCinco(){
  for (int i = tamanhoVetor - 1; i >= 0; i--)
  {
    sem_wait(semaforo);
    if(!(randomNumbers[i] % 5)){
      printf("thread dos multiplos de 5 remove %i\n", randomNumbers[i]);
      remFromVet(randomNumbers, i);
    }
    sem_post(semaforo);
  }
  pthread_exit(NULL);
}

// recua uma casa com todos os elementos depois da posição do elemento a ser removido e diminui o tamanho do vetor
void remFromVet(int *vet, int posBuraco){
  for (int i = posBuraco; i < tamanhoVetor - 1; i++)
  {
    vet[i] = vet[i+1];
  }
  tamanhoVetor--;
}

// diz qual thread é referenciada pelo numero passado
char *threadName(int threadNumber)
{
  char *name = (char *)malloc(sizeof(char) * 30);
  name = (threadNumber % 2 == 0) ? "thread dos pares" : "thread dos múltiplos de 5";
  return name;
}