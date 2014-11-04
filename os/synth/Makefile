include ../Makefile.inc

INCLUDES= -I ../config -I ../lib -I ../midi -I ../system

all:
	$(CC) $(CFLAGS) polyphony.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) voice.c -c  $(INCLUDES)
	ar rcs libqsid-synth.a polyphony.o voice.o

clean:
	rm -f *.o *.a

