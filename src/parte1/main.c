#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

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
    pthread_t identifier = pthread_self();
    printf("Olá, meu nome é %s e meu ID é %lu\n", (char *)name, identifier);
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
            printf("Deu erro na criação de threads: %d", error);
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