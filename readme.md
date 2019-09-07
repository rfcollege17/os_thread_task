##  **PARTE 1**
### Tarefa 1:

#### Fluxo de execução
1. Programa lê do usuario um numero **N**

3. O programa **cria N threads**

5. O programa** imprime para cada** thread, **uma linha** com "*Eu sou a thread **< nome >** e meu ID é **X**"

#### Regras:
1. Não usar variaveis globais
2. Passar o nome da thread como parametro **sem usar formatação de string**

#### Dicas: 
- Crie uma thread com pthread_create()
- O tipo das threads é pthread_t
- Para pegar o id da thread, use pthread_self()

### Tarefa 2:
Altere o programa da **tarefa 1** para imprimir o **ID da thread com pthread_self()** e **com  gettid()**

#### Dicas:
- **pthread_self()** retorna o ID que a **biblioteca pthread** deu para a thread (ID único), já **gettid()** retorna o ID da thread dado pelo **kernel do S.O.**(ID reutilizável)

- Para usar a gettid, importe a biblioteca **#include < sys/syscall.h >** e chame a função com syscall(SYS_gettid)

## PARTE 2

### Instruções

Crie um **vetor global** de inteiros de tamanho **100.000**
Preencher-o com **números aleatórios** entre **1 e 100**

##### Crie 2 threads:
- T1 remove números pares do vetor
- T2 remove números múltiplos de 5 do vetor

####Dicas

- Remova do fim para o começo do vetor e mantenha a ordem original dos elementos
- Teste o programa com e sem semáforos, observe o resultado e explique o que está acontecendo
- Construa uma função que avalia se o resultado final está correto



## Parte 3

Faça o **programa acima** (parte 2) com **processos**. 

1. Use **fork()** para criar um **processo filho** que removerá os **múltiplos de 5**. 

2. Processo pai **remove pares** e verifica se resultado está correto no final. Pai **precisa sincronizar** para esperar filho antes de verificar resultado.

3. É necessário usar ** memória compartilhada**. PDF explica como fazer.

