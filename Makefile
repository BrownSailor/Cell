SRC = src
INC = src/include
BIN = bin

CXX      = clang++
IFLAGS   = -I$(INC)
CXXFLAGS = -g3 -std=c++20 -Wall -Wextra -Wpedantic -Wshadow -Werror $(IFLAGS)
LDFLAGS  = -g3 -Wc

INCLUDES = $(shell echo $(INC)/*.h)
SOURCES = $(shell echo $(SRC)/*.cpp)
OBJECTS = $(SOURCES:$(SRC)/%.cpp=$(BIN)/%.o)

slang: $(OBJECTS)
	@echo Generating executable
	@$(CXX) $(LDFLAGS) -o $@ $^

$(OBJECTS): $(BIN)/%.o: $(SRC)/%.cpp $(INCLUDES)
	mkdir -p $(@D)
	@echo Compiling $< to $@
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo Cleaning binaries
	@rm -rf bin
	@echo Cleaning executable
	@rm slang
