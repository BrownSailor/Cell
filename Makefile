exec = cell
sources = $(wildcard src/*.cpp)
objects = $(sources:.cpp=.o)
flags = -g -std=c++17 -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer

$(exec): $(objects)
	g++ $(objects) $(flags) -o $(exec)

%.o: %.cpp include/%.h
	g++ -c $(flags) $< -o $@

clean:
	-rm src/*.o
	-rm cell
	-rm *.asm
	-rm *.o 
	-rm *.out 