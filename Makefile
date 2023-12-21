INPUT = src/*.c print/libprint.a Itoa/itoa.o

fbi: $(INPUT)
	$(CC) -o fbi $(INPUT)

print/libprint.a:
	make -C print

Itoa/itoa.o:
	make -C Itoa

clean:
	rm -rf *.o fbi
	make clean -C print
	make clean -C Itoa

install: fbi
	install fbi /usr/local/bin
