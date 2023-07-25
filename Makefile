INPUT = basename.c copy.c curl.c default_build.c default_fetch.c default_install.c main.c print/libprint.a

fbi: $(INPUT)
	$(CC) -o fbi $(INPUT)
