CXX = g++
CXXFLAGS = -std=c++20 -O3 -Wall -Wextra -march=native

SRC = src/main.cpp src/lattice_core.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = erebus_core

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o include/*.gch $(TARGET) *.erb

