g: main.o line.o optqueue.o
	cc main.o line.o optqueue.o -o g
main.o: main.c line.h optqueue.h
	cc -Wall -c main.c
line.o: line.c line.h optqueue.h
	cc -Wall -c line.c
optqueue.o: optqueue.c optqueue.h
	cc -Wall -c optqueue.c
clean:
	rm *.o
