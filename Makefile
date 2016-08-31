CC = g++
CFLAGS = -Wall
LIBNP_NAME = libnp.a

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

libnp.a: np.o
	ar rcs libnp.a np.o

libtest: libtest.o libnp.a
	$(CC) $(CFLAGS) -o libtest libtest.o libnp.a

servtest: servtest.o libnp.a
	$(CC) $(CFLAGS) -o servtest servtest.o libnp.a




