CXX = g++

CXXFLAGS = -Wall -ggdb -Iinclude
LDFLAGS =
ECHO = echo

BIN = run
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=build/%.o)

all: $(BIN)

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ $(LDFLAGS)


build/%.o: src/%.cpp
	@if not exist build mkdir build
	@$(CXX) $(CXXFLAGS) -MMD -MF build/$*.d -c $< -o $@

-include $(OBJS:.o=.d)

clean:
	if exist build rd /s /q build
	if exist $(BIN).exe del $(BIN).exe

.PHONY: all clean
