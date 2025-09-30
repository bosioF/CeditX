ifeq ($(OS),Windows_NT)
    TARGET = CeditX.exe
    RM = del /Q
    RUN = $(TARGET)
else
    TARGET = CeditX
    RM = rm -f
    RUN = ./$(TARGET)
endif

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude

SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(TARGET)

run: $(TARGET)
	$(RUN)
