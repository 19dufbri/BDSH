# Simple Makefile, no wildcards because there are only two files

bdsh: bdsh.c bdsh.h
	gcc -std=c11 -Wall $< -o $@

run: bdsh
	bdsh

install: bdsh
	cp bdsh /usr/local/bin/bdsh