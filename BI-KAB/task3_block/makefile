CXX = g++
CXXFLAGS = -std=c++20 -Wall -pedantic -O2 -g
OPENSSL_INCLUDE = ./openssl-3.0.11/include  # Replace this with the actual path to your OpenSSL includes

# List of source files
SRCS = solution.cpp

# Name of the executable
TARGET = program

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -I$(OPENSSL_INCLUDE) -o $(TARGET) $(SRCS) -lcrypto

clean:
	rm -f $(TARGET)