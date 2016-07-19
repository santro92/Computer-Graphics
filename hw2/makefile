# Example 10
EXE=hw2

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
LIBS=-lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
hw2.o: hw2.c CSCIx229.h
special.o: special.c CSCIx229.h
reshape.o: reshape.c CSCIx229.h
axes.o: axes.c CSCIx229.h
print.o: print.c CSCIx229.h
project.o: project.c CSCIx229.h
errcheck.o: errcheck.c CSCIx229.h

#  Create archive
CSCIx229.a: print.o errcheck.o special.o reshape.o axes.o project.o
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -c $(CFLG) $<

#  Link
hw2:hw2.o CSCIx229.a
	gcc -O3 -o $@ $^   $(LIBS)

#  Clean
clean:
	$(CLEAN)
