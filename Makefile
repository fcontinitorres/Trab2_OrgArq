all: clean compile

compile:
	@mkdir -p bin/
	@mkdir -p out/
	gcc src/*.c -o bin/prog -g -Wall -Wextra

run:
	clear && ./bin/prog

memcheck: compile
	valgrind -v --leak-check=full --show-leak-kinds=all --read-var-info=yes --track-origins=yes ./bin/prog

debug:
	gdb ./bin/prog

clean:
	rm -f vgcore*

zip:
	zip -r parte2.zip ./
