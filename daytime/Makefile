# Makefile для клиента daytime

CXX = g++
CXXFLAGS = -Wall -Wextra

TARGET = daytime_client
SRCS = daytime_client.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
