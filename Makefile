CC = cc
CPP = g++
FLAGS = -O2 -Wall --std=c99
CFLAGS = -O2 -Wall
SV_NAME = server
CL_NAME = client
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

# all:

client: $(CL_NAME).cpp
#	$(CPP) $(CFLAGS) -o $@ $?
	$(CPP) $(CFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS) -o $@ $?


server: $(SV_NAME).c
	$(CC) $(FLAGS) -o $@ $?


clean:
	rm -f $(SV_NAME) $(CL_NAME)
