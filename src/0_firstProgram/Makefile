CC := g++
TARGET := firstProgram
LIBS := -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -lGLEW
SOURCES := $(wildcard src/*.cpp src/*/*.cpp src/*/*/*.cpp)

CLEAN_BUILD := rm -rf build
clean:
	rm -rf build
	
.PHONY: all clean

build-all:
	$(CLEAN_BUILD) && mkdir build && g++ -pthread -o build/$(TARGET) $(SOURCES) $(LIBS) && ./build/$(TARGET)