CXX = g++

CXXFLAGS = -Wall -ggdb -Iinclude
LDFLAGS =
ECHO = echo

BIN = ankr
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=build/%.o)

all: $(BIN)

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ $(LDFLAGS)

build/%.o: src/%.cpp
	@mkdir -p build
	@$(CXX) $(CXXFLAGS) -MMD -MF build/$*.d -c $< -o $@

-include $(OBJS:.o=.d)

clean:
	rm -rf build
	rm -f $(BIN)

.PHONY: all clean

