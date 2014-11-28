include ../Makefile.inc

INCLUDES= -I ../config -I ../lib -I ../midi -I ../synth -I ../system -I ../lib/bcm2835/src

all:
	$(CC) $(CFLAGS) midi_init.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) midi_in.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) midi_parser.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) midi_misc.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) midi_udp_in.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) sysex.c -c $(INCLUDES)
	$(CC) $(CFLAGS) sysex_parsers.c -c $(INCLUDES)
	$(CC) $(CFLAGS) sysex_codec.c -c $(INCLUDES)
	ar rcs libqsid-MIDI.a midi_init.o midi_in.o midi_parser.o midi_misc.o midi_udp_in.o sysex.o sysex_parsers.o sysex_codec.o

clean:
	rm -f *.o *.a
