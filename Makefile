# GLOFFVIEW MAKEFILE
# Caleb Brown (c) Copyright 2003
#
# type 'make' to compile
# adjust the flags as necessary

CC = clang
AR = ar
CFLAGS = -Wall -D_GNU_SOURCE # -DDEBUG
LFLAGS = -lGL -lGLU -lglut -L/usr/X11R6/lib

OBJECTS = gloffview.o face.o filereader.o object.o vertex.o render.o trackball.o timer.o

all:	gloffview

gloffview:	$(OBJECTS)
	$(CC) $(LFLAGS) -o gloffview $(OBJECTS)

clean:
	rm -rf *.o gloffview

liteclean:
	rm -rf *.o
