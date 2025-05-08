#Non sono bravo con i makefile, questo l'ho generato usando una webapp
#La potete trovare qua: https://www.nicomedes.assistedcoding.eu/app/makefilegen/c

OBJS	= main.o input.o draw.o list.o snake.o
SOURCE	= main.c input.c draw.c list.c snake.c
HEADER	= lib.h
OUT	= termisnake
FLAGS	= -g -c -Wall
LFLAGS	= 
CC	= gcc

all:	termisnake

termisnake: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

input.o: input.c
	$(CC) $(FLAGS) input.c 

draw.o: draw.c
	$(CC) $(FLAGS) draw.c 

list.o: list.c
	$(CC) $(FLAGS) list.c 

snake.o: snake.c
	$(CC) $(FLAGS) snake.c 

clean:
	rm -f $(OBJS) $(OUT)

run: 
	./termisnake
