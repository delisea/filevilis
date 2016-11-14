all: sfilevilis cfilevilis

sfilevilis: fon.o reseau.o
	gcc -o sfilevilis sfilevilis.c fon.o reseau.o

cfilevilis: fon.o reseau.o
	gcc -o cfilevilis cfilevilis.c fon.o reseau.o

fon.o: fon.h fon.c
	gcc -c fon.c

reseau.o: reseau.h reseau.c fon.o
	gcc -c reseau.c

clean:
	rm *.o sfilevilis cfilevilis
