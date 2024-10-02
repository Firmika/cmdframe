CC := g++

.PHONY: all demo lib clean

all: lib

lib:
	make -C cmdframe

demo: lib
	make -C demo

clean:
	make -C cmdframe clean
	make -C demo clean