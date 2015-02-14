LIBS = -lGL -lGLU -lglut -lm -ljpeg -Wall

all: sphere

clean:
	rm -f *~ sphere

sphere: sphere.c
	gcc $(CFLAGS) sphere.c $(LIBS) -o $@
