INPUT = *.c print/libprint.a

fbi: $(INPUT)
	$(CC) -o fbi $(INPUT)
