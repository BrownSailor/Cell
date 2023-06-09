SRC = src
INC = src/include
BIN = bin

CXX      = clang++
IFLAGS   = -I$(INC)
CXXFLAGS = -g3 -std=c++20 -Wall -Wextra -Wpedantic -Wshadow -Werror $(IFLAGS)
LDFLAGS  = -g3 -Wc -lLLVM

INCLUDES = $(shell echo $(INC)/*.hpp)
SOURCES = $(shell echo $(SRC)/*.cpp)
OBJECTS = $(SOURCES:$(SRC)/%.cpp=$(BIN)/%.o)

slangc: $(OBJECTS)
	@echo Generating executable
	@$(CXX) $(LDFLAGS) -o $@ $^

$(OBJECTS): $(BIN)/%.o: $(SRC)/%.cpp $(INCLUDES)
	mkdir -p $(@D)
	@echo Compiling $< to $@
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo Cleaning binaries
	@rm -rf bin
	@rm -rf *.o
	@echo Cleaning executable
	@rm slangc
