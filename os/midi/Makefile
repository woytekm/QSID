include ../Makefile.inc

INCLUDES= -I ../config -I ../lib -I ../midi -I ../synth -I ../system

all:
	$(CC) $(CFLAGS) init.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) midi_in.c -c  $(INCLUDES)
	ar rcs libqsid-MIDI.a init.o midi_in.o 

clean:
	rm -f *.o *.a
