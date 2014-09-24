include ../Makefile.inc

INCLUDES= -I ../config -I ../lib -I ../midi -I ../synth -I ../system

all:
	$(CC) $(CFLAGS) init.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) midi_in.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) midi_parser.c -c  $(INCLUDES)
	ar rcs libqsid-MIDI.a init.o midi_in.o midi_parser.o

clean:
	rm -f *.o *.a
