#include <stdio.h>
#include <stdlib.h>

void removeEl(int *vet, int *tam, int position)
{
    for (int i = position; i < (*tam) - 1; i++)
    {
        vet[i] = vet[i + 1];
    }
    *tam -= 1;
}
// Retorna 1 se estiver certo, 0 se estiver errado.
int testaParte2(int *original, int tamOriginal, int *resposta, int tamResposta)
{
    int *gabarito = (int *)malloc(sizeof(int) * tamOriginal);
    int tamGabarito = tamOriginal;

    for (int i = 0; i < tamOriginal; i++)
        gabarito[i] = original[i];

    for (int i = tamGabarito - 1; i >= 0; i--)
        if ((!(gabarito[i] % 2)) || (!(gabarito[i] % 5)))
            removeEl(gabarito, &tamGabarito, i);
    
    if (tamGabarito != tamResposta)
    {
        printf("Tamanho não bateu com gabarito\n");
        return 0;
    }

    for (int i = 0; i < tamGabarito; i++)
        if (gabarito[i] != resposta[i])
        {
            printf("%d deveria ser %d\n", resposta[i], gabarito[i]);
            free(gabarito);
            return 0;
        }
    return 1;
}

/*
int main(void) {
    int vet1[] = {1, 2, 3, 4, 5 };
    int resp[] = {3, 1};

    if (testaParte2(vet1, 5, resp, 2))
        printf("Deu bom!\n");
    else
        printf("Deu ruim\n");
}
*/