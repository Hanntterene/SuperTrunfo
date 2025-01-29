main: main.o cartas.o
	@echo "Gerando arquivo executável"
	gcc main.o cartas.o  game.o -o main

main.o: main.c
	@echo "Compilando e gerando o objeto main"
	gcc -c -g main.c

cartas.o: cartas.c
	@echo "Compilando e gerando o objeto cartas"
	gcc -c -g cartas.c

game.o: game.c
	@echo "Compilando e gerando o objeto game"
	gcc -c -g game.c

clean:
	@echo "Apagando objetos e executáveis antigos"
	rm -f *.o main

run:
	./main

debug:
	gdb ./main
