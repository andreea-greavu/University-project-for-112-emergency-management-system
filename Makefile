#GREAVU ANDREEA-MARIA
CC = gcc
CFLAGS = -Wall -Wextra -g

build :main.c functii.c tema1.h
	$(CC) $(CFLAGS) -o tema1 main.c functii.c

run:
	./tema1
clean:
	rm -f tema1

	