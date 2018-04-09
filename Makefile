# Simple Makefile, no wildcards because there are only two files
all: bdsh

bdsh: bdsh.c bdsh.h
	gcc -std=c11 -Wall $< -o $@

run: all
	bdsh

install: all
	cp bdsh /usr/local/bin/bdsh

clean:
	rm -rf bdsh