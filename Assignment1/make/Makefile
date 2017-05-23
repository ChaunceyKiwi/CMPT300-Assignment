# Header files directory
HEADER_DIR = ../include

# Source codes files directory
SRC_DIR = ../src

# The compiler
CC = cc

# The complier option
CFLAG = -g -I$(HEADER_DIR)

# The name of final excutable file
PROG = runTest

OBJS = list.o test.o

run: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

list.o: $(SRC_DIR)/list.c $(HEADER_DIR)/list.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/list.c

test.o: $(SRC_DIR)/test.c $(HEADER_DIR)/list.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/test.c

clean: 
	rm $(PROG) *.o 