# Gateway
#---------------------------------------------------------------------------
CC = gcc
OBJS = Gateway.o Utilities.o LinkedList.o
CFLAGS = -g
LIB = -L/usr/local/lib

#---------------------------------------------------------------------------
all: Gateway
Gateway: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o Gateway $(LIB) -lrt -lpthread -lmulticobex -lbfb -lbluetooth -lobexftp -lopenobex
Gateway.o: Gateway.c Gateway.h
	$(CC) Gateway.c $(CFLAGS) $(LIB) -c
Utilities.o: Utilities.c Utilities.h
	$(CC) Utilities.c $(CFLAGS) $(LIB) -c
LinkedList.o: LinkedList.c LinkedList.h
	$(CC) LinkedList.c $(CFLAGS) $(LIB) -c
clean:
	@rm -rf *.o
