CC = clang
LD = clang
OBJS = ttylog.o read.o write.o

CFLAGS = -pipe -Wall -march=native -O3 -std=c11 -D_GNU_SOURCE

#
# build
#
all: ttylog

ttylog: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

#
# dependency
#
ttylog.o: ttylog.c ttylog.h read.h
read.o: read.c read.h write.h ttylog.h
write.o: write.c write.h

#
# cleanup
#
clean:
	rm -f $(OBJS)
	rm -f ttylog
