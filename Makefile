CC=gcc
CFLAGS=-g -Wall
DEPS=lab1_IO.h timer.h
LIBS=-lm -std=c99 -lpthread

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

demo: main matrixgen
	./matrixgen
	./main 4

main: main.o lab1_IO.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

matrixgen: matrixgen.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o matrixgen data_input data_output main