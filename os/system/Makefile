include ../Makefile.inc

INCLUDES= -I ../config -I ../lib -I ../midi -I ../synth -I ../lib/bcm2835/src

all:
	$(CC) $(CFLAGS) detect.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) sysmisc.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) userio.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) task.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) init.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) patch_mgmt.c -c  $(INCLUDES)
	ar rcs libqsid-system.a sysmisc.o userio.o detect.o task.o init.o patch_mgmt.o

clean:
	rm -f *.o *.a
