TARGET   := ttype
CC       := gcc
CPP		 := g++
CFLAGS   := -std=c99 -O2
CPPFLAGS := -std=c++11 -O2
WARN     := -Wall -Wextra -Wno-format -pedantic
# Libs required can be system-dependent; min is -lglut, max as below.
LIBS    := -lSDL2
OBJECTS := ttype.o

all:	ttype

ttype: ${OBJECTS}
	${CC} ${OBJECTS} -o ttype ${LIBS}

ttype.o: ttype.c
	${CC} ${CFLAGS} ${WARN} -c ttype.c


clean:
	-rm *.o ttype
