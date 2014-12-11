CFLAGS= -DQSID_RPI -I ../lib -I ../system -I ../synth -I ../midi  -I ../config
LIBS = -lncurses
MIDILIBS = -lqsid-MIDI -L ../midi

all:
	gcc -g -o mididump mididump.c  $(CFLAGS)
	gcc -g -o QSID_remote_controller QSID_remote_controller.c  $(CFLAGS) $(LIBS)
	gcc -g -o QSID_remote_controller.v2 QSID_remote_controller.v2.c  $(CFLAGS) $(LIBS) $(MIDILIBS)

clean:
	rm -f *.o mididump QSID_remote_controller

