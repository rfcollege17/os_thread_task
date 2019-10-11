#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "../../header/teste.h"
#include "../../lib/vet/vet.h"

typedef struct vet
{
  int *data;
  int *tam;
} Vet;

void retiraPares(Vet *randomNumbers);
void retiraMultiplosDeCinco(Vet *randomNumbers);
void process_sem_init(struct sembuf *wait, struct sembuf *signal);
void print_error(char *text);
void errorCatch(void * examinatedElement, void *errorCode, char *errorMessage);

long returnCode;
/* Semáforo */
long sem_set_id;
//declarando estruturas de semáforos
struct sembuf process_semWait;
struct sembuf process_semSignal;
/* Memória compartilhada */
long shm_data_id;
void *shm_data_addr;
long shm_tam_id;
void *shm_tam_addr;

Vet *randomNumbers;

int main(void)
{
  //leio o tamanho do vetor desejado
  printf("Por favor, digite o tamanho do vetor aleatório: ");
  int tamanhoLido;
  scanf("%d", &tamanhoLido);

  //inicializo as estruturas do semáforos
  process_sem_init(&process_semWait, &process_semSignal);

  /* Inicializacao do semaforo no sistema. */
  /* Inicia um conjunto de semaforos com 1 elemento */
  sem_set_id = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | S_IRWXU);
  errorCatch((void *) sem_set_id, (void *) -1L, "Erro ao setar o semaforo\n");
  /* inicializa o valor desse semaforo com 1 (por ser Exclusao Mutua). */
  returnCode = semctl(sem_set_id, 0, SETVAL, 1);
  errorCatch((void *) returnCode, (void *) -1L, "Erro ao inicializar o semáforo\n");

  /* Inicializacao da Memoria Compartilhada */
  shm_data_id = shmget(IPC_PRIVATE, (sizeof(int) * tamanhoLido), IPC_CREAT | IPC_EXCL | S_IRWXU);
  errorCatch((void *)shm_data_id, (void *) -1L, "Erro  ao alocar memoria compartilhada\n");

  shm_tam_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRWXU);
  errorCatch( (void *) shm_tam_id, (void *) -1L, "Erro  ao alocar memoria compartilhada\n");

  // dou attach da memoria compartilhada no pai antes de gerar o filho
  shm_data_addr = shmat(shm_data_id, NULL, 0);
  errorCatch(shm_data_addr, NULL, "Erro ao dar attach do vetor no endereço da memoria compartilhada no pai\n");

  shm_tam_addr = shmat(shm_tam_id, NULL, 0);
  errorCatch(shm_tam_addr, NULL, "Erro ao dar attach do tam no endereço da memoria compartilhada no pai\n");

  randomNumbers = (Vet *)malloc(sizeof(Vet));

  randomNumbers->data = (int *)shm_data_addr;
  randomNumbers->tam = (int *)shm_tam_addr;
  *randomNumbers->tam = tamanhoLido;

  //inicializo o vetor com números aleatórios entre 0 e 100
  Vet_put_random(randomNumbers->data, *(randomNumbers->tam));
  Vet inicial;
  inicial.data = Vet_clone(randomNumbers->data, *randomNumbers->tam);
  inicial.tam = (int *) malloc(sizeof(int));
  *inicial.tam = *randomNumbers->tam;

  //mostra o estado do vetor antes das operações
  printf("\e[33mEsse é o vetor original\e[m\n");
  Vet_print(randomNumbers->data, *randomNumbers->tam);

  // BifureturnCodeacao de Processos
  pid_t child_process_id = fork();
  errorCatch((void *) (long) child_process_id, (void *) -1L, "Erro ao usar fork()\n");

  //verifico se é o processo filho (o qeue não tem o id de um filho)
  if(!child_process_id){
    // dou attach da memória compartilhada
    shm_data_addr = shmat(shm_data_id, NULL, 0);
    errorCatch(shm_data_addr, NULL, "deu erro ao dar attach no endereço da memoria compartilhada no filho\n");

    printf("Filho removerá agora multiplos de 5\n");
    retiraMultiplosDeCinco(randomNumbers);
    return 0;
  }
  else{
    printf("Pai removerá agora os pares\n");
    retiraPares(randomNumbers);

    //caso o processo seja o pai, ele espera o filho terminar de executar para morrer
    wait(&child_process_id);

    // printa o vetor com o resultado
    printf("\n\e[33mEsse é o vetor modificado\e[m\n");
    Vet_print(randomNumbers->data, *randomNumbers->tam);

    long respTest = testaRemocaoParalela(inicial.data, *inicial.tam, randomNumbers->data, *randomNumbers->tam);
    errorCatch((void *) respTest, 0L, "Não passou nos testes\n");
    printf("\e[5;32mPassou nos testes\e[m\n");

    /* No fim, remove a memoria compartilhada */
    returnCode = shmctl(shm_data_id, IPC_RMID, NULL);
    errorCatch((void *) returnCode, (void *) -1L, "deu erro ao remover a memoria compartilhada\n");    
  }

}

// retira numeros pares de trás pra frente do vetor
void retiraPares(Vet *randomNumbers)
{
  for (int i = *randomNumbers->tam - 1; i >= 0; i--)
  {
    semop(sem_set_id, &process_semWait, 1);
    if (!(randomNumbers->data[i] % 2))
    {
      printf("Pai remove: %i\n", randomNumbers->data[i]);
      Vet_remove(randomNumbers->data, randomNumbers->tam, i);
    }
    semop(sem_set_id, &process_semSignal, 1);
  }
}

// retira numeros multiplos de 5 de trás pra frente do vetor
void retiraMultiplosDeCinco(Vet *randomNumbers)
{
  for (int i = *randomNumbers->tam - 1; i >= 0; i--)
  {
    semop(sem_set_id, &process_semWait, 1);
    if (!(randomNumbers->data[i] % 5))
    {
      printf("Filho remove: %i\n", randomNumbers->data[i]);
      Vet_remove(randomNumbers->data, randomNumbers->tam, i);
    }
    semop(sem_set_id, &process_semSignal, 1);
  }
}

void print_error(char *text)
{
  printf("\e[31m%s\e[m", text);
}

void process_sem_init(struct sembuf *wait, struct sembuf *signal)
{
  //wait
  wait->sem_num = 0;
  wait->sem_op = -1;
  wait->sem_flg = 0;
  //signal
  signal->sem_num = 0;
  signal->sem_op = 1;
  signal->sem_flg = 0;
}

void errorCatch(void * examinatedElement, void *errorCode, char *errorMessage)
{
  if (examinatedElement == errorCode)
  {
    print_error(errorMessage);
    exit(1);
  }
}