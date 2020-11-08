CC=gcc
CFLAGS=-Wall
DEPS = application.h dcp.h dcp_spec.h logs.h macros.h statemachine.h
OBJ = application.o dcp.o dcp_spec.o logs.o statemachine.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

app: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean: 
	rm -f ./*.o
	rm -f ./app