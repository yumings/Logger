CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp *.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(INCLUDE)/rapidjson-1.1.0/include  -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
