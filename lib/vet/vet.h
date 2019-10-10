#include <stdlib.h>
#include <stdio.h>

#ifndef VETH
#define VETH
typedef struct
{
    int *data;
    int size;
    int capacity;
} Vet;

Vet *Vet_init(int capacity);
void Vet_free(Vet *vet);
// Shows the vet
void Vet_print(Vet *vet);
// Fulfils the given vet with random numbers
void Vet_put_random(Vet *vet);
Vet *Vet_clone(Vet *original);
// Removes even elements from vet and reduces its size accordingly

//funções usadas pelo teste
void Vet_remove_pairs(Vet *vet);
void Vet_remove_multiples_five(Vet *vet);
void Vet_remove(Vet *vet, int position);

#endif