CFLAGS = 
LDFLAGS =
LIBS =

TARGET = dumpedlastbuf

SRC = dumpedlastbuf.c
OBJ = ${SRC:.c=.o}

all: $(TARGET)
	
.c.o:
	@echo CC $<
	${CC} -c ${CFLAGS} $<
	
${TARGET}: ${OBJ}
	@echo CC -o $@
	${CC} -o $@ ${OBJ} ${LDFLAGS} ${LIBS}

