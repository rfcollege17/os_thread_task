compiler = gcc
default_libs = -pthread

p1t1:
	@ echo "Parte 1 - Tarefa 1"
	@ echo "Compilando..."
	@ $(compiler) $(default_libs) src/parte1/tarefa1.c -o bin/p1t1.bin
	@ echo "Executando:\n"
	@ ./bin/p1t1.bin

p1t2:
	@ echo "Parte 1 - Tarefa 2" 
	@ echo "Compilando..."
	@ $(compiler) $(default_libs) src/parte1/tarefa2.c -o bin/p1t2.bin
	@ echo "Executando:\n"
	@ ./bin/p1t2.bin

p2:
	@ echo "Parte 2" 
	@ echo "Compilando..."
	@ $(compiler) $(default_libs) src/parte2/*.c -o bin/p2.bin
	@ echo "Executando:\n"
	@ ./bin/p2.bin