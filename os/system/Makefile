INCLUDES= -I ../config -I ../lib -I ../midi -I ../synth

all:
	gcc detect.c -c  $(INCLUDES)
	gcc sysmisc.c -c  $(INCLUDES)
	gcc userio.c -c  $(INCLUDES)
	ar rcs libqsid-system.a sysmisc.o userio.o detect.o

clean:
	rm -f *.o *.a
