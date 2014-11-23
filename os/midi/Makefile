include ../Makefile.inc

INCLUDES= -I ../config -I ../lib -I ../midi -I ../synth -I ../system -I ../lib/bcm2835/src

all:
	$(CC) $(CFLAGS) init.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) midi_in.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) midi_parser.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) midi_misc.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) midi_udp_in.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) sysex.c -c $(INCLUDES)
	$(CC) $(CFLAGS) sysex_parsers.c -c $(INCLUDES)
	ar rcs libqsid-MIDI.a init.o midi_in.o midi_parser.o midi_misc.o midi_udp_in.o sysex.o sysex_parsers.o

clean:
	rm -f *.o *.a
