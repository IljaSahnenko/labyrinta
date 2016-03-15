CC = cc
# LIBS =
FLAGS = -O2 -w --std=c99
NAME = server


server: $(NAME).c
	$(CC) $(FLAGS) -o $@ $? $(LIBS)

clean:
	rm $(NAME)
