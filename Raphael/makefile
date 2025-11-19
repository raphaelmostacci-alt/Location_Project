CC=gcc
CFLAGS=-Wall -Wextra -std=c99

SRC=meeting_room.c client_management.c administrator.c room_management.c reservation_room.c useful_fonction.c useful_mutual.c
OBJ=$(SRC:.c=.o)

EXEC=meeting_room

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
