CC = g++
CFLAGS = -std=c++17 -Iinclude `pkg-config --cflags libpcap libnl-3 libnl-genl-3`
LDFLAGS = `pkg-config --libs libpcap libnl-3 libnl-genl-3`

SRC = \
    src/main.cpp \
    src/network_capture.cpp \
    src/packet_injection.cpp \
    src/interface_monitor.cpp \
    src/log_manager.cpp

OBJ = $(SRC:.cpp=.o)

TARGET = NetworkTool

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
