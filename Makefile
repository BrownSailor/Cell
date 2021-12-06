build:
	g++ -std=c++17 -o cell src/*.cpp

clean:
	-rm cell
	-rm *.asm
	-rm *.o 