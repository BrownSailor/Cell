exec = a.out
sources = $(wildcard src/*.cpp)
objects = $(sources:.cpp=.o)
flags = -g -std=c++17 -Wall

$(exec): $(objects)
	g++ $(objects) $(flags) -o $(exec)

%.o: %.cpp include/%.h
	g++ -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.o