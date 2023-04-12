generator: generator.c 3d.o
	gcc -Wall -Werror -std=c11 -o generator  generator.c 3d.o -lm
3d.o: 3d.c 3d.h
	gcc -Wall -Werror -std=c11  -c 3d.c  -o 3d.o
clean:
	rm -f 3d.o generator
