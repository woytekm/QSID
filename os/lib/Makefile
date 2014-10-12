include ../Makefile.inc

INCLUDES= -I ../config -I ../lib -I ../midi -I ../synth -I ../system

libqsid-i2c:
	$(CC) $(CFLAGS) i2c_open.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) i2c_lib.c -c $(INCLUDES)
	ar rcs libqsid-i2c.a i2c_open.o i2c_lib.o

libqsid-SID:
	$(CC) $(CFLAGS) SID_lowlevel.c -c $(INCLUDES)
	$(CC) $(CFLAGS) SID_control.c -c $(INCLUDES)
	$(CC) $(CFLAGS) SID_remote_control.c -c $(INCLUDES)
	$(CC) $(CFLAGS) SID_writer.c -c $(INCLUDES)
	ar rcs libqsid-SID.a  SID_lowlevel.o SID_control.o SID_remote_control.o SID_writer.o

all:
	$(CC) $(CFLAGS) i2c_open.c -c  $(INCLUDES)
	$(CC) $(CFLAGS) i2c_lib.c -c $(INCLUDES)
	$(CC) $(CFLAGS) SID_lowlevel.c -c $(INCLUDES)
	$(CC) $(CFLAGS) SID_control.c -c $(INCLUDES)
	$(CC) $(CFLAGS) SID_remote_control.c -c $(INCLUDES)
	$(CC) $(CFLAGS) SID_writer.c -c $(INCLUDES)
	ar rcs libqsid-i2c.a i2c_open.o i2c_lib.o
	ar rcs libqsid-SID.a  SID_lowlevel.o SID_control.o SID_remote_control.o SID_writer.o


clean:
	rm -f *.o *.a

