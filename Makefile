build:
	clang++ -O3 -std=c++17 -o slang src/*.cpp -Wall -Werror -Wextra

clean:
	-rm slang
	-rm *.asm
	-rm *.o
