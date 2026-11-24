CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -g

SOURCES := $(shell find EDs -type f -name '*.c' -print)
INCLUDES := $(shell find EDs -type d -print | sed 's/^/-I/')
TARGET := dado_burger
PEDIDOS := TXTs/pedidos.txt
HISTORICO := TXTs/historico.txt

.PHONY: all game-build game-run clean run-game

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -lm -o $(TARGET)

game-build: $(TARGET)

run-game: game-run

game-run: $(TARGET)
	./$(TARGET) $(PEDIDOS) $(HISTORICO)

clean:
	rm -f $(TARGET)
