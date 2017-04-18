# Time-stamp: <02/11/09 10:40:24 INGA>
# File: Makefile

COMPILE=g++
LINK=g++
CARG= -c -O4 -Wall -std=c++11

DESTDIR=/
PREFIX=$(DESTDIR)/usr/local
DEPDIR=.deps


OBJECTa= main.o 
OBJECTb= nurn.o 
OBJECTc= address.o 
OBJECTd= communication.o 
OBJECTe= udpcommunication.o 
OBJECTf= socket.o 
OBJECTg= udpsocket.o 

targeta=main


all: $(OBJECTa) $(OBJECTb) $(OBJECTc) $(OBJECTd) $(OBJECTe) $(OBJECTf) $(OBJECTg)
	$(COMPILE) -o $(targeta) $(OBJECTa) $(OBJECTb) $(OBJECTc) $(OBJECTd) $(OBJECTe) $(OBJECTf) $(OBJECTg)

clean:
	rm -f *.o *.exe
	rm -r $(targeta)

install: 
	install -m 0755 main $(PREFIX)/bin

main.o: Main.cpp
	$(COMPILE) $(CARG) Main.cpp
	
nurn.o: Nurn.hpp Nurn.cpp
	$(COMPILE) $(CARG) Nurn.cpp
	
address.o: Address.hpp Address.cpp 
	$(COMPILE) $(CARG) Address.cpp
	
communication.o: Communication.hpp Communication.cpp
	$(COMPILE) $(CARG) Communication.cpp
	
udpcommunication.o: UDPCommunication.hpp UDPCommunication.cpp
	$(COMPILE) $(CARG) UDPCommunication.cpp
	
socket.o: Socket.hpp Socket.cpp
	$(COMPILE) $(CARG) Socket.cpp
	
udpsocket.o: UDPSocket.hpp UDPSocket.cpp
	$(COMPILE) $(CARG) UDPSocket.cpp
