# Makefile для клиента echo

CXX = g++
CXXFLAGS = -Wall -Wextra

TARGET = echo_client
SRCS = echo_client.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
