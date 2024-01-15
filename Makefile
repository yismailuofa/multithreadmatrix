CC=gcc
CFLAGS=-g -Wall -std=c99 -lm
DEPS=lab1_IO.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

demo: main matrixgen
	./matrixgen
	./main 4

main: main.o lab1_IO.o
	$(CC) -o $@ $^ $(CFLAGS) -lpthread

matrixgen: matrixgen.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o matrixgen data_input data_output main