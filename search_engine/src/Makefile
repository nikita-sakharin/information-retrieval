CXX=g++
RM=rm -frd
CXXFLAGS=-std=c++17 -fexceptions -m64 -Wall -Werror -Weffc++ -Wextra -pedantic \
    -Wfatal-errors -pedantic-errors -Winit-self \
    -Wzero-as-null-pointer-constant -Wnon-virtual-dtor -Winline \
    -Wmissing-declarations -Wmissing-include-dirs -Wundef -Wredundant-decls \
    -Wfloat-equal -Wmain -Wunreachable-code -Wshadow -Wcast-align \
    -Wswitch-default -Wswitch-enum
LDFLAGS=-m64
LDLIBS=-lm
SOURCES=$(wildcard *.cpp)
HEADERS=$(wildcard ../include/search_engine/*.hpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

debug: CXXFLAGS+=-Og -g
debug: all
release: CXXFLAGS+=-fomit-frame-pointer -fexpensive-optimizations -flto -O3
release: LDFLAGS+=-O3 -flto -s
release: all

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -I../include -o $@

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
