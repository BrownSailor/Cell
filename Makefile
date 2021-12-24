build:
	gcc -o cell src/*.c

clean:
	-rm cell
	-rm *.asm
	-rm *.o 
	-rm *.out