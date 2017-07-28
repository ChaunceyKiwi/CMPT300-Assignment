# Header files directory
HEADER_DIR = ../include

# Source codes files directory
SRC_DIR = ../src

# The compiler
CC = cc

# The complier option
CFLAGS = -Wall -g -I$(HEADER_DIR)

# The name of final excutable file
PROG = UnixLs

OBJS = UnixLs.o

run: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

UnixLs.o: $(SRC_DIR)/UnixLs.c $(HEADER_DIR)/UnixLs.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/UnixLs.c

clean: 
	rm $(PROG) *.o 