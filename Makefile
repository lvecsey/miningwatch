
CC=gcc

CFLAGS=-O3 -Wall -g -pg

all : sysrq_boot miningwatch

sysrq_boot : sysrq_boot.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)	

miningwatch : miningwatch.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)
