#include "vet.h"

int *Vet_init(int tam)
{
  return malloc(sizeof(int) * tam);
}

// Shows the vet
void Vet_print(int *vet, int tam)
{
  printf("\n");
  for (int i = 0; i < tam; i++)
    printf("%d, ", vet[i]);
  printf("\n\n");
}

// Fulfils the given vet with random numbers
void Vet_put_random(int *vet, int tam)
{
  printf("Oi\n");
  for (int i = 0; i < tam; i++)
  {
    vet[i] = rand() % 100;
  }
}

int *Vet_clone(int *original, int tam)
{
  int *clone = Vet_init(tam);
  for (int i = 0; i < tam; i++)
    clone[i] = original[i];
  return clone;
}

void Vet_remove(int *vet, int *tam, int position)
{
  if (!vet || position >= *tam)
    return;

  for (int i = position; i < *tam - 1; i++)
  {
    vet[i] = vet[i + 1];
  }
  (*tam)--;
}

static void Vet_remove_multiples(int *vet, int *tam, int divisor)
{
  for (int i = (*tam) - 1; i >= 0; i--)
    if ((!(vet[i] % divisor)))
      Vet_remove(vet, tam, i);
}

// Removes even elements from vet and reduces its size accordingly
void Vet_remove_pairs(int *vet, int *tam)
{
  Vet_remove_multiples(vet, tam, 2);
}

void Vet_remove_multiples_five(int *vet, int *tam)
{
  Vet_remove_multiples(vet, tam, 5);
}
