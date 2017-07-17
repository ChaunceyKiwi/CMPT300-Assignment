# Header files directory
HEADER_DIR = ../include

# Source codes files directory
SRC_DIR = ../src

# The compiler
CC = cc

# The complier option
CFLAGS = -Wall -g -I$(HEADER_DIR) -pthread

# The name of final excutable file
PROG = s-talk

OBJS = list.o s-talk.o

run: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

list.o: $(SRC_DIR)/list.c $(HEADER_DIR)/list.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/list.c

s-talk.o: $(SRC_DIR)/s-talk.c $(HEADER_DIR)/list.h $(HEADER_DIR)/s-talk.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/s-talk.c

clean: 
	rm $(PROG) *.o 