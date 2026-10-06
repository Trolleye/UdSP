CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2 -g

# Target executable name
TARGET = main

# Object files placed inside the build directory
OBJS = build/main.o build/linked_list.o build/cpu_load.o

.PHONY: all clean

all: $(TARGET)

# Link the object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rule to compile .c files into the build/ directory
build/%.o: %.c | build
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
build:
	mkdir -p build

# Clean up build artifacts and executable
clean:
	rm -rf build $(TARGET)