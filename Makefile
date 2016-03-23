CC = cc
CPP = g++
FLAGS = -O2 -Wall --std=c99
CFLAGS = -O2 -lSDL2 -Wall
SV_NAME = server
CL_NAME = client

all:

server: $(SV_NAME).c
	$(CC) $(FLAGS) -o $@ $?

client: $(CL_NAME).cpp
	$(CPP) $(CFLAGS) -o $@ $?

clean:
	rm $(NAME)
