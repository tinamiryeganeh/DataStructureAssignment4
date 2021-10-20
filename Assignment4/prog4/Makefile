FLAG=-std=c11 -Wall

prog4: Collide.o QTree.o
	gcc $(FLAG) -o prog4 Collide.o QTree.o -lm

Collide.o: Collide.c QTree.h
	gcc $(FLAG) -c -g Collide.c

QTree.o: QTree.h QTree.c
	gcc $(FLAG) -c -g QTree.c

clean:
	rm *.o prog4

check:
	valgrind --leak-check=full -v ./prog4