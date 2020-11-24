CXX=g++
RM=rm -frd
CXXFLAGS=-fexceptions -m64 -pedantic-errors -std=c++17 -Wall -Wcast-align \
    -Wdouble-promotion -Weffc++ -Werror -Wextra -Wfatal-errors -Wfloat-equal \
    -Winit-self -Winline -Wmain -Wmissing-declarations -Wmissing-include-dirs \
    -Wnon-virtual-dtor -Wnull-dereference -Wpadded -Wpedantic \
    -Wredundant-decls -Wshadow -Wswitch-default -Wswitch-enum -Wundef \
    -Wuninitialized -Wunreachable-code -Wzero-as-null-pointer-constant
LDFLAGS=-m64
LDLIBS=-lm
SOURCES=$(filter-out ../src/main.cpp, $(wildcard ../src/*.cpp))
TEST_SOURCES=$(wildcard *.test.cpp)
HEADERS=$(wildcard ../include/search_engine/*.hpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

debug: CXXFLAGS+=-Og -g
debug: all
release: CXXFLAGS+=-O3 -fexpensive-optimizations -flto -fomit-frame-pointer -s
release: LDFLAGS+=-O3 -fexpensive-optimizations -flto -fomit-frame-pointer -s
release: all

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -I../include -o $@

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
