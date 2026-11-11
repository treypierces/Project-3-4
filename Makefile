CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -g -MMD -MP

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=build/%.o)
TARGET := ./bin/main

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	$(CXX) $^ -o $@

build/%.o: %.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build bin:
	mkdir -p $@

run: $(TARGET)
	./$(TARGET) $(ARGS)

clean:
	rm -rf ./build/ ./bin/

-include $(OBJS:.o=.d)

.PHONY: all clean