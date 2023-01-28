CC := gcc
TARGET := imgConv
CFLAGS = -std=c99 -I $(IDIR) -Wall -Wextra -lpng -lm -g3

ODIR := ./obj
SDIR := ./src
IDIR := ./inc

_DEPS := argParser.h imgProcessing.h arg.h
DEPS := $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ := main.o argParser.o imgProcessing.o
OBJ := $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $^ $(CFLAGS)

.PHONY: mem clean

mem:
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --track-origins=yes --num-callers=20 --track-fds=yes ./$(TARGET) -i ./lena_std.png -o ./out.txt -g 255 -vd -s 320 320
	
clean:
	rm -f $(ODIR)/*.o $(TARGET)