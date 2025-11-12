CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = 348Lab11
OBJS = main.o Matrix.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

main.o: main.cpp Matrix.h
Matrix.o: Matrix.cpp Matrix.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)