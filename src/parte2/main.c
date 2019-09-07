#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#define tamanhoVetor 100

void printaVetor();
void inicializaVetor(int * vet, int tamanho);
void criaThreads(int * vet, int tamanho);
void retiraPares();
void remFromVet(int *vet, int tamanho, int posBuraco);
void retiraMultiplosDeCinco();
char *threadName(int threadNumber);

int randomNumbers[tamanhoVetor];

void main(){
  inicializaVetor(randomNumbers, tamanhoVetor);
  printaVetor();

  criaThreads(randomNumbers, tamanhoVetor);

  printaVetor();
  pthread_exit(NULL);
}

// printa o vetor no estado atual
void printaVetor(){
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

// gera as 2 threads e oque cada uma vai fazer
void criaThreads(int * vet, int tamanho){
  int errorCode;
  void * callback;
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
  }
}

// retiro numeros pares de trás pra frente do vetor
void retiraPares(){
  for (int i = tamanhoVetor; i >= 0; i--)
  {
    // começo região critica
    if((randomNumbers[i]%2) == 0){
      printf("thread dos pares remove: %i\n", randomNumbers[i]);
      remFromVet(randomNumbers, tamanhoVetor, i);
    }
    // fim região critica
  }
  pthread_exit(NULL);
}

// retira numeros multiplos de 5 de trás pra frente do vetor  
void retiraMultiplosDeCinco(){
  for (int i = tamanhoVetor; i >= 0; i--)
  {
    // começo de região critica
    if((randomNumbers[i] % 5) == 0){
      printf("thread dos multiplos de 5 remove %i\n", randomNumbers[i]);
      remFromVet(randomNumbers, tamanhoVetor, i);
    }
    // fim de região critica
  }
  pthread_exit(NULL);
}

// recua uma casa com todos os elementos depois do elemento a ser removiso
void remFromVet(int *vet, int tamanho, int posBuraco){
  for (int i = posBuraco; i < tamanho - 1; i++)
  {
    vet[i] = vet[i+1];
  }
  vet[tamanho-1] = -1;
}

// diz qual thread é referenciada pelo numero passado
char *threadName(int threadNumber)
{
  char *name = (char *)malloc(sizeof(char) * 30);
  name = (threadNumber % 2 == 0) ? "thread dos pares" : "thread dos múltiplos de 5";
  return name;
}