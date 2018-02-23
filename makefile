COMPILE_FLAGS = -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual \
		-Wcast-align -Wwrite-strings -Wnested-externs -Winline \
		-W -Wundef -Wmissing-prototypes -O0 -g
.SUFFIXES: .c .o
LINK_FLAGS =
LIBS = -lm -lc

CC = gcc

all:    fibonacci

clean:
	rm -f *~
	rm -f core
	rm -f *.o
	rm -f fibonacci

%.o: %.c
	$(CC) $(COMPILE_FLAGS) -c $< -o $@

fibonacci: fibonacci.o
	$(CC) $(LINK_FLAGS) $(LIBS) -o $@ $^
