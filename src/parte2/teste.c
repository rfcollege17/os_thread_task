#include <stdio.h>
#include <stdlib.h>
#include "../../lib/vet/vet.h"

int testaParte2(Vet *original, Vet *resposta)
{
    Vet *gabarito = Vet_clone(original);

    Vet_remove_pairs(gabarito);
    Vet_remove_multiples_five(gabarito);
    
    if (gabarito->size != resposta->size)
    {
        printf("Tamanho não bateu com gabarito\n");
        return 0;
    }

    for (int i = 0; i < gabarito->size; i++)
        if (gabarito->data[i] != resposta->data[i])
        {
            printf("%d deveria ser %d\n", resposta->data[i], gabarito->data[i]);
            free(gabarito);
            return 0;
        }
    return 1;
}
