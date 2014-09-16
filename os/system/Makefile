include ../Makefile.inc

INCLUDES= -I ../config -I ../lib -I ../midi -I ../synth

all:
	$(CC) $(CFLAGS) detect.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) sysmisc.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) userio.c -c  $(INCLUDES)
	ar rcs libqsid-system.a sysmisc.o userio.o detect.o

clean:
	rm -f *.o *.a
