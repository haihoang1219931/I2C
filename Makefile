IDIR := include
I2CDIR := i2c
CC := gcc
CFLAGS := -I$(IDIR)

OBJECTS := i2c_example.o $(IDIR)/$(I2CDIR)/*.o
TARGETS := i2c_example
i2c_example: i2c_example.o $(IDIR)/$(I2CDIR)/*.o
	$(CC) $(OBJECTS) -o $(TARGETS)

i2c_example.o:
	$(CC) $(CFLAGS) -c i2c_example.c -o i2c_example.o

smbus.o:  
	cd $(IDIR)/$(I2CDIR)
	$(CC) -c smbus.c -o smbus.o
clean:
	rm -f $(OBJECTS) $(TARGETS)
re: 	
	rm -f $(OBJECTS) $(TARGETS) 
	make
