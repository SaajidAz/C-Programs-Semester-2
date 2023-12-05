#makefile
labt: labt.o functions.o
	g++ -g -o labt labt.o functions.o
labt.o: labt.c functions.h
	g++ -g -c labt.c
functions.o: functions.c functions.h
	g++ -g -c functions.c
clean:
	touch *
	rm *.o

