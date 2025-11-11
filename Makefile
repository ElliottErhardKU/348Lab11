CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = 348Lab11

all: $(TARGET)

$(TARGET): 348Lab11.cpp
	$(CXX) $(CXXFLAGS) 348Lab11.cpp -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
