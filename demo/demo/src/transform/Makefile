LIBS = -lGL -lGLU -lglut -lm -ljpeg -Wall

all: transform

clean:
	rm -f *~ transform

transform: transform.c
	gcc $(CFLAGS) transform.c $(LIBS) -o $@
