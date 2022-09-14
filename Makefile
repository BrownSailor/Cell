build:
	clang++ -O3 -std=c++17 -o slang src/*.cpp -Wall -Werror -Wextra

debug:
	clang++ -O0 -g -std=c++17 -o slang src/*.cpp -Wall -Werror -Wextra  -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment

clean:
	-rm slang
	-rm *.asm
	-rm *.o 