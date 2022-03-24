build:
	clang++ -Ofast -std=c++17 -o slang src/*.cpp -Wall -Werror -Wextra -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment

clean:
	-rm slang
	-rm *.asm
	-rm *.o 
	-rm *.out