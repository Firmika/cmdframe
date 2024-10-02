CC := g++
INCLUDE := -I include
LIB := -lmenu
CFLAG := -std=c++11
OBJS := frameutils.o cmdframe.o cmdmenu.o cmdframe_manager.o

.PHONY: all clean

all: $(OBJS)
	ar rcs ../libcmdframe.a $(OBJS) $(LIB)

%.o: %.cpp
	$(CC) $(CFLAG) -c $< -o $@ $(INCLUDE)

clean:
	-rm -f *.o
	-rm -f ../libcmdframe.a