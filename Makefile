CXX      = clang++
CXXFLAGS = -g3 -std=c++20 -Ofast -Wall -Wextra -Wpedantic -Wshadow -Werror
LDFLAGS  = -g3

SRC = src
INC = src/include
BIN = bin
OBJ = $(BIN)/obj

SOURCE_FILES  = lexer.cpp parser.cpp error.cpp builtin.cpp assembler.cpp \
				compiler.cpp util.cpp main.cpp

OBJECT_FILES  = $(SOURCE_FILES:%.cpp=$(OBJ)/%.o)

build: slang

test: slang
	python test.py

clean:
	@echo Cleaning binaries
	@rm -rf bin
	@echo Cleaning executable
	@rm slang

slang: $(OBJECT_FILES)
	@echo Generating executable
	@$(CXX) $(LDFLAGS) -o $@ $^

$(OBJECT_FILES): $(OBJ)/%.o: $(SRC)/%.cpp $(INC)/%.h
	@mkdir -p $(@D)
	@echo Compiling $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $@
