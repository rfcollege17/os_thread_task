#include "../../header/vet.h"


Vet *Vet_init(int capacity)
{
    Vet *new = (Vet *) malloc(sizeof(Vet));
    new->capacity = capacity;
    new->size = 0;
    new->data = (int *) malloc(sizeof(int) * capacity);
    return new;
}

void Vet_free(Vet *vet)
{
    free(vet->data);
    free(vet);
}

// Shows the vet
void Vet_print(Vet *vet)
{
    printf("\n");
    for (int i = 0; i < vet->size; i++)
        printf("%d, ", vet->data[i]);
    printf("\n\n");
}

// Fulfils the given vet with random numbers
void Vet_put_random(Vet *vet)
{
    for (int i = 0; i < vet->size; i++)
    {
        vet->data[i] = rand() % 100;
    }
}

Vet *Vet_clone(Vet *original)
{
    Vet *clone  = Vet_init(original->capacity);
    for (int i = 0; i < original->size; i++)
        clone->data[i] = original->data[i];
    clone->size = original->size;
    return clone;
}

void Vet_remove(Vet *vet, int position)
{
    if (!vet || position >= vet->size)
        return;

    for (int i = position; i < vet->size - 1; i++)
    {
        vet->data[i] = vet->data[i + 1];
    }
    vet->size--;
}

static void Vet_remove_multiples(Vet *vet, int divisor)
{
    for (int i = vet->size - 1; i >= 0; i--)
        if ((!(vet->data[i] % divisor)))
            Vet_remove(vet, i);
}

// Removes even elements from vet and reduces its size accordingly
void Vet_remove_pairs(Vet *vet)
{
    Vet_remove_multiples(vet, 2);
}

void Vet_remove_multiples_five(Vet *vet)
{
    Vet_remove_multiples(vet, 5);
}
