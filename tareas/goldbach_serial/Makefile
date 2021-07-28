APPNAME=goldbach

CC=cc
CFLAGS=-g -std=gnu11 -Wall -Wextra
LIBS=-pthread

$(APPNAME): $(APPNAME).c
	$(CC) $(CFLAGS) $(APPNAME).c -o $(APPNAME) $(LIBS)

all: $(APPNAME) lint memcheck helgrind

.PHONY: lint
lint: 
	cpplint --filter=-readability/casting goldbach_serial.c

.PHONY: memcheck
memcheck: 
	valgrind --tool=memcheck ./$(APPNAME)
	
.PHONY: helgrind
helgrind: 
	valgrind --tool=helgrind ./$(APPNAME)
	
.PHONY: clean	
clean:
	rm -f $(APPNAME)