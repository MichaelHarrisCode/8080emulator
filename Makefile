# Figure out how to make this dynamic and fun
# Perhaps, I could have an array that holds the
# file names, then I somehow do everything else
# dynamically? =D 

output: main.o 8080emu.o 8080dis.o 8080xtra.o
	gcc main.o 8080emu.o 8080dis.o 8080xtra.o -o output

main.o: main.c
	gcc -c main.c

8080emu.o: 8080emu.c
	gcc -c 8080emu.c

8080dis.o: 8080dis.c
	gcc -c 8080dis.c

8080xtra.o: 8080xtra.c
	gcc -c 8080xtra.c

clean:
	rm *.o output