include ../Makefile.inc

INCLUDES= -I ../config -I ../lib -I ../midi -I ../system -I ../lib/bcm2835/src

all:
	$(CC) $(CFLAGS) polyphony.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) voice.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) lfo.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) patch.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) silencer.c -c $(INCLUDES)
	ar rcs libqsid-synth.a polyphony.o voice.o lfo.o patch.o silencer.o

clean:
	rm -f *.o *.a

