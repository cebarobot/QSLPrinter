TARGET = main

cc = gcc
LD = gcc

CFLAGS = -Wall -Iinclude
LDFlags = 

LIBS = 

HDRS = ./include/*.h

SRCS = main.c printer.c

OBJS = $(patsubst %.c,%.o,$(SRCS))

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET) $(LIBS) 

$(OBJS) : %.o : %.c include/*.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(TARGET)
