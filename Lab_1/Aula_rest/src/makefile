CC = gcc
CFLAGS = -Wall
DEPS = macros.h
OBJ = 
TARGETS = writenoncanonical noncanonical

all: writenoncanonical noncanonical

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

writenoncanonical: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm

noncanonical: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm

clean:
	@rm $(TARGETS)

# @rm *.o $(TARGETS)  em cima caso comece a criar *.o