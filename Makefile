CC = gcc
CFLAGS = -Wall
SRCS = main.c utils/hashmap.c config.c command.c
TARGET = pmgr
INSTALL_PATH = /usr/local/bin

.PHONY: install clean

install:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)
	sudo cp $(TARGET) $(INSTALL_PATH)/$(TARGET)

clean:
	rm -f $(TARGET)
