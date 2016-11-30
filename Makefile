CC = cc
CPP = g++
CFLAGS = -O2 -Wall --std=c11
CPPFLAGS = -O2 -Wall
SRCPATH=./src
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image

all: client server example menu mmm

client:
	$(CPP) $(SRCPATH)/client.cpp $(CPPFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS) -o $@ $?

menu:
	$(CPP) $(SRCPATH)/menu.cpp $(CPPFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS) -o $@ $?

mmm:
	$(CPP) $(SRCPATH)/menu.cpp $(CPPFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS) -o $@ $?

example:
	$(CC) $(SRCPATH)/example_client.c $(CFLAGS) -o $@ $?

server:
	$(CC) $(SRCPATH)/server.c $(CFLAGS) -o $@ $?

clean:
	rm -f server client example menu mmm
