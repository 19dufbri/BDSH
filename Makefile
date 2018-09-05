# Simple Makefile, no wildcards because there are only two files

IDIR   = ./include
ODIR   = ./bin
SDIR   = ./src
OUT    = bdsh
CFLAGS = -std=c11 -I$(IDIR) -Wall
CC     = gcc
HEAD   = $(IDIR)/bdsh.h $(IDIR)/builtin.h
OBJ    = $(ODIR)/bdsh.o $(ODIR)/builtin.o

all: $(OUT)

$(ODIR)/%.o: ./src/%.c $(HEAD) $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(ODIR):
	mkdir $(ODIR)

run: all
	./$(OUT)

install: all
	cp $(OUT) /usr/local/bin/$(OUT)

clean:
	rm -rf $(OUT) $(ODIR)