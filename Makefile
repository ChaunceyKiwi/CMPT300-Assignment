# The compiler
CC = cc

# The complier option
CFLAG = -g

# The name of final excutable file
PROG = runTest

OBJS = list.o test.o

run: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

.c.o: 
	$(CC) $(CFLAGS) -c $*.c

list.o: list.h

test.o: list.h

clean: 
	rm *.o 