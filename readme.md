##  **PARTE 1**
### Tarefa 1:

#### Fluxo de execução
1. Programa lê do usuario um numero **N**

3. O programa **cria N threads**

5. O programa** imprime para cada** thread, **uma linha** com "*Eu sou a thread **< nome >** e meu ID é **X**"

#### Regras:
1. Não usar variaveis globais
2. Passar o nome da thread como parametro sem usar formatação de string

#### Dicas: 
- Crie uma thread com pthread_create()
- O tipo das threads é pthread_t
- Para pegar o id da thread, use pthread_self()

### Tarefa 2:
Altere o programa da **tarefa 1** para imprimir o **ID da thread com pthread_self()** e **com  gettid()**

#### Dicas:
- **pthread_self()** retorna o ID que a thread tem dentro do processo, já **gettid(**) retorna o ID da thread dado pelo S.O.

- Para usar a gettid, importe a biblioteca **#include < sys/syscall.h >** e chame a função com syscall(SYS_gettid)