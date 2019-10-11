#include <stdio.h>
#include <stdlib.h>
#include "../lib/vet/vet.h"

int testaRemocaoParalela(int *original, int tamOriginal, int *resposta, int tamResposta)
{
  int *gabarito = Vet_clone(original, tamOriginal);
  int tamGabarito = tamOriginal;

  Vet_remove_pairs(gabarito, &tamGabarito);
  Vet_remove_multiples_five(gabarito, &tamGabarito);

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
  free(gabarito);
  return 1;
}
