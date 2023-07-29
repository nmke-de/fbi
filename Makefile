INPUT = *.c print/libprint.a

fbi: $(INPUT)
	$(CC) -o fbi $(INPUT)

clean:
	rm -rf *.o fbi

install: fbi
	install fbi /usr/local/bin
