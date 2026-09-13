CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
SRC_DIR := src
BIN_DIR := bin
TARGET := $(BIN_DIR)/physics_engine

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

web:
	./build_web.sh

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)
	rm -f web/physics_engine.js web/physics_engine.wasm web/physics_engine.data

run: all
	./$(TARGET)

serve-web:
	cd web && python3 -m http.server 8080

.PHONY: all web clean run serve-web
