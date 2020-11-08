CC=gcc
CFLAGS=-Wall
DEPS = application.h dcp.h dcp_spec.h logs.h macros.h statemachine.h
OBJ = application.o dcp.o dcp_spec.o logs.o statemachine.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

app: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

.PHONY: clean

clean: 
	rm -f ./*.o
	rm -f ./app