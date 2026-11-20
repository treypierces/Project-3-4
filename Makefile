CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -g -MMD -MP -fsanitize=address -fno-omit-frame-pointer -O0
LDFLAGS := -g -fsanitize=address -lsfml-graphics -lsfml-window -lsfml-system

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=build/%.o)
TARGET := ./bin/main

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	$(CXX) $(LDFLAGS) $^ -o $@

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