CC := g++
LIB := -L ../ -lcmdframe -lncurses -lmenu
INCLUDE := -I ../cmdframe/include/
CFLAG := -std=c++11

.PHONY: all clean

all: demo.o
	$(CC) $(CFLAG) $(INCLUDE) demo.o -o ../_demo $(LIB)

demo.o:
	$(CC) $(CFLAG) $(INCLUDE) -c demo.cpp -o demo.o

clean:
	-rm -f demo.o
	-rm -f ../_demo