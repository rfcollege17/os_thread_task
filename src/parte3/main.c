#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "../../header/parte3/teste.h"
#include "../../lib/vet/vet.h"

#define TAM 100000;

int rc; // return code

// Semáforo
int sem_set_id;
/* Inicializacao das estruturas para manipular o semaforo */
struct sembuf process_semWait = {
    .sem_num = 0,
    .sem_op = -1,
    .sem_flg = 0};

struct sembuf process_semSignal = {
    .sem_num = 0,
    .sem_op = 1,
    .sem_flg = 0};

// Memória compartilhada
int shm_id;
void *shm_addr;

int main(void)
{
  /* Inicializacao do semaforo no sistema. Requer iniciar
  um conjunto de semaforos com 1 elemento e inicializar o
  valor desse semaforo com 1 (no caso da Exclusao Mutua). */

  sem_set_id = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | S_IRWXU);
  if (sem_set_id == -1)
  {
    printf("deu ruim quando fui criar o sem_set\n");
    exit(1);
  }

  rc = semctl(sem_set_id, 0, SETVAL, 1);
  if (rc == -1)
  {
    printf("Deu ruim ao inicializar o semáforo\n");
    exit(1);
  }

  /* A partir disso, e so usar "wait" e "signal" para
  manipular o semaforo antes e depois da zona crítica. */
}

// Memoria compartilhada
// shm_id = shmget(100, sizeof(int) * TAM, IPC_CREAT | IPC_EXCL | 0600);
// if (shm_id == -1)
// {
//   printf("deu erro na memoria compartilhada\n");
//   exit(1);
// }

// shm_addr = shmat(shm_id, NULL, 0);
// if (!shm_addr)
// {
//   printf("deu ruim no endereço da memoria compartilhada\n");
//   exit(1);
// }

// Birfucacao de Processos
// pid_t child_process = fork();

// switch (child_process)
// {
// case -1:
//   printf("Deu merda\n");
//   return -1;
// case 0:
//   printf("Sou o filhinho\n");
//   return 0;
// default:
//   wait(&child_process);
//   printf("Papai chegou\n");
//   return 0;
// }