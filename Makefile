#
#	Makefile for OpenGL example from Chapter 1 in Red Book
#

CC = g++ 
CFLAGS = -D_DEBUG
CPPFLAGS = -g

.cpp.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@  $<

#LIBS = -framework OpenGL -framework GLUT -lglew 

LIBS = -lGL -lGLU -lglut -lm -ljpeg -Wall -lGLEW

SRCS = example1.cpp LoadShaders.cpp
OBJS = example1.o LoadShaders.o 

example1: $(OBJS) LoadShaders.h
	g++ -g -o example1 $(OBJS) $(LIBS)

clean:
	rm -f example1 *.o
