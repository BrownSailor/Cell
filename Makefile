CXX      = clang++
CXXFLAGS = -g3 -std=c++20 -Ofast -Wall -Wextra -Wpedantic -Wshadow -Werror
LDFLAGS  = -g3 -Wc -fsanitize=address -fsanitize=undefined

SRC = src
INC = src/include
BIN = bin

INCLUDES = $(shell echo $(INC)/*.h)
SOURCES = $(shell echo $(SRC)/*.cpp)
OBJECTS = $(SOURCES:$(SRC)/%.cpp=$(BIN)/%.o)

build: slang

$(OBJECTS): $(BIN)/%.o: $(SRC)/%.c $(INCLUDES)
	mkdir -p $(@D)
	@echo Compiling $< to $@
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo Cleaning binaries
	@rm -rf bin
	@echo Cleaning executable
	@rm slang

slang: $(OBJECT_FILES)
	@echo Generating executable
	@$(CXX) $(LDFLAGS) -o $@ $^

$(OBJECT_FILES): $(BIN)/%.o: $(SRC)/%.cpp $(INC)/%.h
	@mkdir -p $(@D)
	@echo Compiling $< to $@
	@$(CXX) $(CXXFLAGS) -c $< -o $@
