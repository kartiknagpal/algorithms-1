CC = gcc
OBJECTS = main.o powers_of/power.o
TARGETS = algorithms

all: $(TARGETS)

%.o: %.c
	$(CC) -Wall -std=gnu99 -c -g -o $@ $<

algorithms: $(OBJECTS)
	$(CC) -o algorithms $(OBJECTS)

clean:
	@echo Cleaning up.
	rm -f *.o
	rm algorithms
	@echo Done.
