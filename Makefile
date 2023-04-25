SRC = src
INC = src/include
BIN = bin

CXX      = clang++
IFLAGS   = -I$(INC)
CXXFLAGS = -g3 -Ofast -std=c++20 -Wall -Wextra -Wpedantic -Wshadow -Werror $(IFLAGS)
LDFLAGS  = -g3 -Ofast -Wc

INCLUDES = $(shell echo $(INC)/*.hpp)
SOURCES = $(shell echo $(SRC)/*.cpp)
OBJECTS = $(SOURCES:$(SRC)/%.cpp=$(BIN)/%.o)

slang: $(OBJECTS)
	@echo Generating executable
	@$(CXX) $(LDFLAGS) -o $@ $^
	@echo Linking external dependencies
	@./scripts/link.sh

$(OBJECTS): $(BIN)/%.o: $(SRC)/%.cpp $(INCLUDES)
	mkdir -p $(@D)
	@echo Compiling $< to $@
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo Cleaning binaries
	@rm -rf bin
	@echo Cleaning libraries
	@rm -rf std/lib
	@rm -rf *.o
	@echo Cleaning executable
	@rm slang
