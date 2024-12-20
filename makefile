main: main.o cartas.o
	@echo "Gerando arquivo executável"
	gcc main.o cartas.o -o main

main.o: main.c
	@echo "Compilando e gerando o objeto main"
	gcc -c main.c

cartas.o: cartas.c
	@echo "Compilando e gerando o objeto cartas"
	gcc -c cartas.c

clean:
	@echo "Apagando objetos e executáveis antigos"
	rm -f *.o main

run:
	./main