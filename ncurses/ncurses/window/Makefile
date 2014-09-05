
CUR_DIR=${shell pwd}
SRC_DIR=.
#EXE_DIR=${CUR_DIR}
EXE_DIR=.


CC=gcc
CFLAGS=-I/usr/include/ncurses
CFLAGS+=-DCURSES_LOC="<ncurses.h>"
LDFLAGS=-lncurses -lmenu -lform

ifeq ($(strip $(WINDOWS)),y)
	CFLAGS+=-DWINDOWSCMD
endif


EXE=${EXE_DIR}/main
OBJS=${shell echo *.c|sed 's/\.c\>/.o/g'}


${EXE} : ${OBJS} 
	${CC} -o $@ $^ ${LDFLAGS}

$(OBJS) : %.o : ${SRC_DIR}/%.c
	${CC} -o $@ -c $< ${CFLAGS}


.PHONY : clean
clean:
	-rm *.o *.d *.exe *.txt 2>/dev/null


-include make.deprules
-include $(OBJS:.o=.d)

