INPUT = *.c print/libprint.a

fbi: $(INPUT)
	$(CC) -o fbi $(INPUT)

print/libprint.a:
	make -C print

clean:
	rm -rf *.o fbi

install: fbi
	install fbi /usr/local/bin
