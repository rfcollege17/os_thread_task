#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

void *helloThread(void *name);
int getNumberOfThreads(void);
void createThreads(int numberOfThreads);
char *threadName(int threadNumber);

int main(void)
{
    int numberThreads = getNumberOfThreads();
    createThreads(numberThreads);
    pthread_exit(NULL);
}

void *helloThread(void *name)
{
    pthread_t identifier_by_pthread = pthread_self();
    pthread_t identifier_by_getId = syscall(SYS_gettid);
    printf("Olá, meu nome é \e[33m%s\e[m. meu ID com \e[32mpthread_self: %lu\e[m e com \e[34mgetID: %lu\e[m\n", (char *)name, identifier_by_pthread, identifier_by_getId);
    free(name);
    pthread_exit(NULL);
}

int getNumberOfThreads(void)
{
    int numberThreads;
    printf("Por favor, digite o número de Threads desejado: ");
    scanf("%d", &numberThreads);
    return numberThreads;
}

void createThreads(int numberOfThreads)
{
    int error;
    for (int i = 0; i < numberOfThreads; i++)
    {
        pthread_t currentThread;
        error = pthread_create(&currentThread, NULL, helloThread, threadName(i));
        if (error)
        {
            printf("\e[31mDeu erro na criação de threads: %d\e[m\n", error);
            exit(1);
        }
    }
}

char *threadName(int threadNumber)
{
    char *name = (char *)malloc(sizeof(char) * 20);
    sprintf(name, "thread_%i", threadNumber);
    return name;
}