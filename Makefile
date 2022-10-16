CXX      = clang++
CXXFLAGS = -g3 -std=c++20 -Ofast -Wall -Wextra -Wpedantic -Wshadow -Werror
LDFLAGS  = -g3

SRC = src
INCLUDE = src/include

slang: main.o compiler.o assembler.o builtin.o parser.o lexer.o
	${CXX} ${LDFLAGS} -o slang $^

main.o: $(SRC)/main.cpp $(INCLUDE)/compiler.h $(INCLUDE)/assembler.h $(INCLUDE)/builtin.h $(INCLUDE)/parser.h $(INCLUDE)/lexer.h
	$(CXX) $(CXXFLAGS) -c src/main.cpp

compiler.o: $(SRC)/compiler.cpp $(INCLUDE)/compiler.h $(INCLUDE)/assembler.h $(INCLUDE)/builtin.h $(INCLUDE)/parser.h $(INCLUDE)/lexer.h
	$(CXX) $(CXXFLAGS) -c src/compiler.cpp

assembler.o: $(SRC)/assembler.cpp $(INCLUDE)/assembler.h $(INCLUDE)/builtin.h $(INCLUDE)/parser.h $(INCLUDE)/lexer.h
	$(CXX) $(CXXFLAGS) -c src/assembler.cpp

builtin.o: $(SRC)/builtin.cpp $(INCLUDE)/builtin.h $(INCLUDE)/parser.h $(INCLUDE)/lexer.h
	$(CXX) $(CXXFLAGS) -c src/builtin.cpp

parser.o: $(SRC)/parser.cpp $(INCLUDE)/parser.h $(INCLUDE)/lexer.h
	$(CXX) $(CXXFLAGS) -c src/parser.cpp

lexer.o: $(SRC)/lexer.cpp $(INCLUDE)/lexer.h
	$(CXX) $(CXXFLAGS) -c src/lexer.cpp

clean:
	-rm slang
	-rm *.o
	-rm *.cc
	-rm main
