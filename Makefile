CC = clang
LD = clang
OBJS = uartlog.o read.o write.o

CFLAGS = -pipe -Wall -march=native -O3 -std=c11 -D_GNU_SOURCE

#
# build
#
all: uartlog

uartlog: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

#
# dependency
#
uartlog.o: uartlog.c uartlog.h read.h
read.o: read.c read.h write.h uartlog.h
write.o: write.c write.h

#
# cleanup
#
clean:
	rm -f $(OBJS)
	rm -f uartlog
