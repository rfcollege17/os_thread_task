#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "../../header/parte3/teste.h"
#include "../../lib/vet/vet.h"

void retiraPares(Vet *randomNumbers);
void retiraMultiplosDeCinco(Vet *randomNumbers);
void print_error(char *text);

int rc; // return code

/* Semáforo */
int sem_set_id;

//declarando estruturas de semáforos
struct sembuf process_semWait;
struct sembuf process_semSignal;

/* Memória compartilhada */
int shm_data_id;
void *shm_data_addr;
int shm_tam_id;
void *shm_tam_addr;

Vet *randomNumbers;

int main(void)
{

  process_semWait.sem_num = 0;
  process_semWait.sem_op = -1;
  process_semWait.sem_flg = 0;

  process_semSignal.sem_num = 0;
  process_semSignal.sem_op = 1;
  process_semSignal.sem_flg = 0;

  printf("Por favor, digite o tamanho do vetor aleatório: ");
  int tam;
  scanf("%d", &tam);

  /* Inicializacao do semaforo no sistema. */
  /* Inicia um conjunto de semaforos com 1 elemento */
  sem_set_id = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | S_IRWXU);
  if (sem_set_id == -1)
  {
    print_error("deu ruim quando fui criar o sem_set\n");
    exit(1);
  }

  /* inicializa o valor desse semaforo com 1 (por ser Exclusao Mutua). */
  rc = semctl(sem_set_id, 0, SETVAL, 1);
  if (rc == -1)
  {
    print_error("Deu ruim ao inicializar o semáforo\n");
    exit(1);
  }

  /* Inicializacao da Memoria Compartilhada */
  shm_data_id = shmget(IPC_PRIVATE, sizeof(int) * tam, IPC_CREAT | IPC_EXCL | S_IRWXU);
  if (shm_data_id == -1)
  {
    print_error("deu erro  ao alocar memoria compartilhada\n");
    exit(1);
  }

  shm_tam_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRWXU);
  // Nao vou tratar o erro
  shm_tam_addr = shmat(shm_tam_id, NULL, 0);
  // Tbm nao

  // dou attach da memoria compartilhada no pai antes de gerar o filho
  shm_data_addr = shmat(shm_data_id, NULL, 0);
  if (!shm_data_addr)
  {
    print_error("deu erro ao dar attach no endereço da memoria compartilhada no pai\n");
    exit(1);
  }

  randomNumbers = (Vet *)malloc(sizeof(Vet));
  randomNumbers->data = shm_data_addr;
  randomNumbers->size = randomNumbers->capacity = tam;
  int *tam_addr = shm_tam_addr;
  *tam_addr = tam;
  Vet_put_random(randomNumbers);

  printf("\e[33mEsse é o vetor original\e[m\n");
  Vet_print(randomNumbers);

  // Bifurcacao de Processos
  pid_t child_process = fork();

  switch (child_process)
  {
  case -1:
    print_error("Erro ao usar fork()");
    return -1;
  case 0:
    // dou attach da memoria compartilhada no filho
    shm_data_addr = shmat(shm_data_id, NULL, 0);
    if (!shm_data_addr)
    {
      print_error("deu erro ao dar attach no endereço da memoria compartilhada no filho\n");
      exit(1);
    }
    printf("Filho removerá agora multiplos de 5\n");
    retiraMultiplosDeCinco(randomNumbers);
    return 0;
  default:
    printf("Pai removerá agora os pares\n");
    retiraPares(randomNumbers);
  }

  //caso o processo seja o pai, ele espera o filho terminar de executar para morrer
  if (child_process)
  {
    wait(&child_process);
    printf("\n\e[33mEsse é o vetor modificado\e[m\n");
    int *tam_addr = shm_tam_addr;
    int tam = *tam_addr;
    randomNumbers->size = tam;
    Vet_print(randomNumbers);

    /* No fim, remover a memoria compartilhada */
    rc = shmctl(shm_data_id, IPC_RMID, NULL); 
    if (rc == -1)
    {
      print_error("deu erro ao remover a memoria compartilhada\n");
      exit(1);
    }
  }
}

// retira numeros pares de trás pra frente do vetor
void retiraPares(Vet *randomNumbers)
{
  for (int i = randomNumbers->size - 1; i >= 0; i--)
  {
    semop(sem_set_id, &process_semWait, 1);
    if (!(randomNumbers->data[i] % 2))
    {
      printf("Pai remove: %i\n", randomNumbers->data[i]);
      Vet_remove(randomNumbers, i);
      int *tam_addr = shm_tam_addr;
      *tam_addr -= 1;
    }
    semop(sem_set_id, &process_semSignal, 1);
  }
}

// retira numeros multiplos de 5 de trás pra frente do vetor
void retiraMultiplosDeCinco(Vet *randomNumbers)
{
  for (int i = randomNumbers->size - 1; i >= 0; i--)
  {
    semop(sem_set_id, &process_semWait, 1);
    if (!(randomNumbers->data[i] % 5))
    {
      printf("Filho remove: %i\n", randomNumbers->data[i]);
      Vet_remove(randomNumbers, i);
      int *tam_addr = shm_tam_addr;
      *tam_addr -= 1;
    }
    semop(sem_set_id, &process_semSignal, 1);
  }
}

void print_error(char *text)
{
  printf("\e[31m%s\e[m", text);
}