# Makefile to compile and run main.c

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -lm

# Target executable
TARGET = main

# Source files
SRC = main.c

# Build target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

# Clean target
clean:
	rm -f $(TARGET)

# Run target
run: $(TARGET)
	./$(TARGET)