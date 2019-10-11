#include <stdlib.h>
#include <stdio.h>

#ifndef VETH
#define VETH


int *Vet_init(int tam);
// Shows the vet
void Vet_print(int *vet, int tam);
// Fulfils the given vet with random numbers
void Vet_put_random(int *vet, int tam);
int *Vet_clone(int *original, int tam);

//funções usadas pelo teste
void Vet_remove_pairs(int *vet, int *tam);
void Vet_remove_multiples_five(int *vet, int *tam);

void Vet_remove(int *vet, int *tam, int position);

#endif