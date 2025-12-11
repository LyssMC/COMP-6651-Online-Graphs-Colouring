CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -std=c11
LIBS    := -lm
LDFLAGS := $(LIBS)

SRC     := graph.c generator.c firstfit.c cbip.c main.c
OBJ     := $(SRC:.c=.o)
TARGET  := program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)
