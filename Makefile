CC      := gcc
CFLAGS  := -std=c23 -Wall -Wextra -Wpedantic -Werror
TARGET  := tush
SOURCE  := tush.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

debug:
	$(CC) $(CFLAGS) -DDEBUG -o $(TARGET) $(SOURCE) 
clean:
	rm -f $(TARGET)
