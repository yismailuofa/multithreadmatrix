CC=gcc
CFLAGS=-g -Wall -std=gnu99
DEPS=lab1_IO.h timer.h
LIBS=-lm -lpthread

demo: main matrixgen
	./matrixgen
	./main 4

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


main: main.o lab1_IO.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
matrixgen: matrixgen.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o matrixgen data_input data_output main