all: anap4.c kinelib.o startup.o dataclient.o histdef.o anaevt.o anap4.h
	$(CC) -o anap4 anap4.c kinelib.o startup.o dataclient.o \
                histdef.o anaevt.o -lm -lpacklib -lmathlib -lpthread -lncurses

startup.o: startup.c startup.h anap4.h userdef.h
	$(CC) -c startup.c

histdef.o: histdef.c anap4.h userdef.h
	$(CC) -c histdef.c

anaevt.o: anaevt.c anap4.h kinema.h userdef.h
	$(CC) -c anaevt.c

kinelib.o: kinelib.c kinema.h
	$(CC) -c kinelib.c

dataclient.o: dataclient.c dataclient.h common.h
	$(CC) -c dataclient.c

clean:
	rm -f anap4 *.o



