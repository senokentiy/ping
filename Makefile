CC = cc
CFLAGS = -O2 -Wall -Wextra 


.PHONY: all clean

all: clean ping

ping: ping.o
	$(CC) $(CFLAGS) $^ -o $@ 

ping.o: ping.c ping.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f ping.o ping 

