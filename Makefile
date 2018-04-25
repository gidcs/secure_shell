
CC	= gcc
CXX	= g++ -std=c++11
CFLAGS = -Wall -g -static

# PROGS = main
PROGS = main
OBJ = main.o src/func.o
C_PLUS = 1

all: $(PROGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(PROGS): $(OBJ)
ifeq (${C_PLUS}, 1)
	$(CXX) $(CFLAGS) $(OBJ) -o $@
else
	$(CC) $(CFLAGS) $(OBJ) -o $@
endif

clean:
	rm -f *~ $(PROGS) $(OBJ)

