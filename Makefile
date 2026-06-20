CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

SRC = $(wildcard src/*.cpp)
TARGET = build/database

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf build

run: all
	./$(TARGET)
