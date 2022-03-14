#To Run Code For Server -: $make -> ./server
#To Run Code For Client -: $make -> ./client

#variable declaration :-
cc=gcc      
MAKE=make
RM =rm

#Flags
FLAG = -Wall

#targets .
all: client11.c server11.c
	$(cc) $(FLAG) -o client client11.c	
	$(cc) $(FLAG) -o server server11.c -pthread -lbsd
	
clean:server client
	$(RM) *.o
