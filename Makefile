CC=gcc
CFLAGS=-g -Wall

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

matrixgen: matrixgen.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o matrixgen data_input