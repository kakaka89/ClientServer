CC=g++
FLAG=-g

all:client server

client:client.cpp ClientClass.h ClientClass.cpp
	$(CC) $(FLAG) -o client client.cpp ClientClass.h ClientClass.cpp


server:server.cpp ServerClass.h ServerClass.cpp
	$(CC) $(FLAG) -o server server.cpp ServerClass.h ServerClass.cpp -lpthread

clean:
	rm -f client server
 