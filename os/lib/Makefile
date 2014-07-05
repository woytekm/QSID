INCLUDES= -I ../config -I ../lib -I ../midi -I ../synth

all:
	gcc i2c_open.c -c  $(INCLUDES)
	gcc i2c_lib.c -c $(INCLUDES)
	ar rcs libqsid-i2c.a i2c_open.o i2c_lib.c

clean:
	rm -f *.o *.a

