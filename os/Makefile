# Q-SID OS main makefile

include Makefile.inc

INCLUDES     = -I config -I lib -I midi -I synth -I system
DIRS         = control display lib midi synth system 
MYLIBS       = libqsid-i2c.a libqsid-system.a libqsid-SID.a libqsid-MIDI.a libqsid-synth.a
MYLIBDIRS    = -Llib -Lmidi -Lsynth -Lsystem -Lcontrol -Ldisplay
SYSLIBS      = 
SYSLIBDIRS   =
LDOPTS       = -lqsid-system -lqsid-MIDI -lqsid-synth -lqsid-i2c -lqsid-SID -lpthread
OSEXE        = qsid-os


all:	$(MYLIBS) $(OSEXE)


$(OSEXE):
	$(CC) $(CFLAGS) os-main.c -c $(INCLUDES) 
	$(CC) $(CFLAGS) -o $(OSEXE) os-main.o $(LDOPTS) $(MYLIBDIRS) $(SYSLIBS) $(SYSLIBDIRS) 


libqsid-i2c.a:
	cd lib; $(MAKE)  

libqsid-SID.a:
	cd lib; $(MAKE)

libqsid-system.a:
	cd system; $(MAKE)

libqsid-MIDI.a:
	cd midi; $(MAKE)

libqsid-synth.a:
	cd synth; $(MAKE)


clean:
	rm -f *.o *.a $(OSEXE)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done
