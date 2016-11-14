all: bin/sfilevilis bin/cfilevilis

bin/sfilevilis: fon.o reseau.o
	gcc -o bin/sfilevilis sfilevilis.c fon.o reseau.o

bin/cfilevilis: fon.o reseau.o
	gcc -o bin/cfilevilis cfilevilis.c fon.o reseau.o

fon.o: fon.h fon.c
	gcc -c fon.c

reseau.o: reseau.h reseau.c fon.o
	gcc -c reseau.c

clean:
	rm *.o bin/sfilevilis bin/cfilevilis
