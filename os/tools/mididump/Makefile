CFLAGS= -DQSID_RPI -I ../../lib -I ../../system -I ../../synth -I ../../midi  -I ../../config
LIBS = -lncurses
MIDILIBS = -lqsid-MIDI -lqsid-system -L ../../midi -L ../../system

all:
	gcc -g -o mididump mididump.v2.c  $(CFLAGS) $(MIDILIBS)

clean:
	rm -f *.o mididump 

