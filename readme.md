Parte 1
  Tarefa 1:
    1- programa lê do usuario um numero N

    2- o programa cria N threads

    3- o programa imprime para cada thread, uma linha com "Eu sou a thread <nome> e meu ID é tal"

    Regras:
        1- não usar variaveis globais
        2- passar o nome da thread como parametro sem usar formatação de string

    Dicas: 
        1- crie uma thread com pthread_create()
        2- o tipo das threads é pthread_t
        3- para pegar o id da thread, use pthread_self()
  
  Tarefa 2:
    Altere o programa da tarefa 1 para imprimir o ID da thread com pthread_self() e com  gettid()

    Dicas:
      1- pthread_self() retorna o ID que a thread tem dentro do processo,
      já gettid() retorna o ID da thread dado pelo S.O.

      2- para usar a gettid, importe a biblioteca <sys/syscall.h> e chame a função
      com syscall(SYS_gettid)