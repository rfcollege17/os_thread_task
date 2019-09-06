compiler = gcc

parte1:
	@ echo "Parte 1"
	@ echo "Compilando..."
	@ $(compiler) -pthread src/parte1/*.c -o bin/parte1.bin
	@ echo "Executando:\n"
	@ ./bin/parte1.bin