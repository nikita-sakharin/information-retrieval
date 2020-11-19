CXX=g++
CXXFLAGS=-std=c++17 -fexceptions -m64 -Wall -Werror -Weffc++ -Wextra -pedantic \
    -Wfatal-errors -pedantic-errors -Winit-self \
    -Wzero-as-null-pointer-constant -Wnon-virtual-dtor -Winline \
    -Wmissing-declarations -Wmissing-include-dirs -Wundef -Wredundant-decls \
    -Wfloat-equal -Wmain -Wunreachable-code -Wshadow -Wcast-align \
    -Wswitch-default -Wswitch-enum
LDFLAGS=-m64
LDLIBS=-lm
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

debug: CXXFLAGS+=-Og -g
debug: $(SOURCES) $(EXECUTABLE)
release: CXXFLAGS+=-fomit-frame-pointer -fexpensive-optimizations -flto -O3
release: LDFLAGS+=-O3 -flto -s
release: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -frd $(OBJECTS) $(EXECUTABLE)
