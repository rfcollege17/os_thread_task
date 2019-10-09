#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "../../header/parte3/teste.h"
#include "../../lib/vet/vet.h"

#define TAM 100000;

int rc;

int sem_set_id;
int shm_id;
void *shm_addr;

int main(void)
{
  shm_id = shmget(100, sizeof(int) * TAM, IPC_CREAT | IPC_EXCL | 0600);
  if (shm_id == -1)
  {
    printf("deu erro na memoria compartilhada\n");
    exit(1);
  }

  shm_addr = shmat(shm_id, NULL, 0);
  if (!shm_addr)
  {
    printf("deu ruim no endereço da memoria compartilhada\n");
    exit(1);
  }

  sem_set_id = semget(IPC_PRIVATE, 1, 0600);
  if (sem_set_id == -1)
  {
    printf("deu ruim quando fui criar o sem_set\n");
    exit(1);
  }

  semctl(sem_set_id, 0, SETVAL, 1);
  if (rc == -1)
  {
    printf("Deu ruim ao inicializar o semáforo\n");
    exit(1);
  }

  return 0;
  pid_t child_process = fork();

  switch (child_process)
  {
  case -1:
    printf("Deu merda\n");
    return -1;
  case 0:
    printf("Sou o filhinho\n");
    return 0;
  default:
    wait(&child_process);
    printf("Papai chegou\n");
    return 0;
  }
}