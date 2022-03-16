build:
	clang++ -Ofast -std=c++17 -o cell src/*.cpp -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment

clean:
	-rm cell
	-rm *.asm
	-rm *.o 
	-rm *.out