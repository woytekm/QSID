# Q-SID OS main makefile

include Makefile.inc

INCLUDES     = -I config -I lib -I midi -I synth -I system
DIRS         = control display lib midi synth system 
OBJECTS      = libqsid-i2c.a libqsid-system.a
MYLIBS       = -lqsid-i2c -lqsid-system
MYLIBDIRS    = -Llib -Lmidi -Lsynth -Lsystem -Lcontrol -Ldisplay
SYSLIBS      = 
SYSLIBDIRS   =
OSEXE        = qsid-os


all:	$(OBJECTS) $(OSEXE)


$(OSEXE):
	gcc os-main.c -c  $(CFLAGS) $(INCLUDES) 
	gcc -o $(OSEXE) os-main.o $(MYLIBS) $(MYLIBDIRS) $(SYSLIBS) $(SYSLIBDIRS) 


libqsid-i2c.a:
	cd lib; $(MAKE)  


libqsid-system.a:
	cd system; $(MAKE)


clean:
	rm -f *.o *.a $(OSEXE)
	for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done
