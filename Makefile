all: compile run clean

compile:
	@mkdir -p bin/
	gcc src/registro.c src/indice.c src/menu.c src/main.c -o bin/prog -g && clear

run:
	./bin/prog

memcheck:
	valgrind -v --leak-check=full --show-leak-kinds=all --read-var-info=yes --track-origins=yes ./bin/prog

debug:
	gdb ./bin/prog

clean:
	rm -f vgcore*

zip:
	zip -r parte2.zip ./
