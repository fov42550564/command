# Makefile for win 

# A few variables

CC=gcc
LDFLAGS=-lncurses -lpanel
CFLAGS=-I/usr/include/ncurses

CUR_DIR=${shell pwd}
SRC_DIR=${CUR_DIR}
EXE_DIR=${CUR_DIR}

EXES = \
	${EXE_DIR}/xx

${EXE_DIR}/%:%.o 
	${CC} -o $@ $< ${LDFLAGS}

%.o: ${SRC_DIR}/%.c
	${CC} -o $@ -c $< ${CFLAGS}

all: ${EXES}

clean:
	@rm -f ${EXES}
