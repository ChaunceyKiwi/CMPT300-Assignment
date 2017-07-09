# Header files directory
HEADER_DIR = ../include

# Source codes files directory
SRC_DIR = ../src

# The compiler
CC = cc

# The complier option
CFLAGS = -Wall -g -I$(HEADER_DIR)

# The name of final excutable file
PROG = simulation

OBJS = list.o simulation.o

run: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

list.o: $(SRC_DIR)/list.c $(HEADER_DIR)/list.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/list.c

simulation.o: $(SRC_DIR)/simulation.c $(HEADER_DIR)/list.h $(HEADER_DIR)/simulation.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/simulation.c

clean: 
	rm $(PROG) *.o 