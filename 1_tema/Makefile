SOURCES := homework.c
EXECUTABLES := $(SOURCES:.c=)
CC := gcc
CFLAGS := -Wall -O3 -lm -lpthread 

all: $(EXECUTABLES)
$(EXECUTABLES): % : %.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f $(EXECUTABLES)