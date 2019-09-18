compiler = gcc
default_libs = -pthread

p1t1: build_p1t1
	@ echo "Executando:\n"
	@ ./bin/p1t1

build_p1t1:
	@ echo "Parte 1 - Tarefa 1"
	@ echo "Compilando..."
	@ $(compiler) $(default_libs) src/parte1/p1t1.c -o bin/p1t1

p1t2: build_p1t2
	@ echo "Executando:\n"
	@ ./bin/p1t2

build_p1t2:
	@ echo "Parte 1 - Tarefa 2" 
	@ echo "Compilando..."
	@ $(compiler) $(default_libs) src/parte1/p1t2.c -o bin/p1t2

p2: build_p2
	@ echo "Executando:\n"
	@ ./bin/p2
	
build_p2:
	@ echo "Parte 2" 
	@ echo "Compilando..."
	@ $(compiler) $(default_libs) src/parte2/*.c -o bin/p2
