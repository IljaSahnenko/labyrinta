CC = cc
FLAGS = -O2 -Wall --std=c99
NAME = server


server: $(NAME).c
	$(CC) $(FLAGS) -o $@ $?

clean:
	rm $(NAME)
