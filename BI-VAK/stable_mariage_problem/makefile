CXX = g++
CXXFLAGS = -std=c++20 -Wall -pedantic -O2 -g

# List of source files
SRCS = main.cpp

# Name of the executable
TARGET = program

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)