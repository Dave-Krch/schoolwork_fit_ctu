CXX = g++
CXXFLAGS = -g -std=c++17 -Wall -pedantic -Ilibs
LD = g++
LDFLAGS =

SRC=$(wildcard src/*.cpp)
HDR=$(wildcard src/*.h)
OBJ=$(patsubst src/%.cpp, build/%.o, $(SRC))

all: krchdavi doc

krchdavi: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

compile: krchdavi

run: krchdavi
	./krchdavi

build/%.o: src/%.cpp build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir build

-include build/Makefile.d

build/Makefile.d: build $(SRC) $(HDR)
	$(CXX) -MM $(SRC) > $@

clean:
	rm -rf krchdavi doc build

doc:
	doxygen Doxyfile