CC = gcc
CFLAGS = -Wall -std=c99 -g
LDFLAGS = -lm

all:	encode	decode

encode:	encode.o	filebuffer.o	state24.o
encode.o:	encode.c	filebuffer.h	state24.h	
filebuffer.o:	filebuffer.c	filebuffer.h
state24.o:	state24.c	state24.h

decode:	decode.o	filebuffer.o	state24.o
decode.o:	decode.c	filebuffer.h	state24.h		
filebuffer.o:	filebuffer.c	filebuffer.h
state24.o:	state24.c	state24.h

clean:	
	rm -f	encode	decode	state24.o	filebuffer.o	encode.o	decode.o	output.txt





	