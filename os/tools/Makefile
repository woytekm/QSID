CFLAGS= -DQSID_RPI -I ../lib -I ../system -I ../synth -I ../midi  -I ../config
LIBS = -lncurses

all:
	gcc -g -o mididump mididump.c  $(CFLAGS)
	gcc -g -o QSID_remote_controller QSID_remote_controller.c  $(CFLAGS) $(LIBS)

clean:
	rm -f *.o mididump QSID_remote_controller

