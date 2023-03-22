CC := gcc
TARGET := imgConv
CFLAGS = -std=c99 -I $(IDIR) -Wall -Wextra -lpng -lm -O3

ODIR := ./obj
SDIR := ./src
IDIR := ./inc

_DEPS := argParser.h imageHandler.h image.h imageProcessing.h fileHandler.h
DEPS := $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ := main.o argParser.o imageHandler.o image.o imageProcessing.o fileHandler.o
OBJ := $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $^ $(CFLAGS)

.PHONY: mem clean

mem:
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --track-origins=yes --num-callers=20 --track-fds=yes ./$(TARGET) -i ./lena_std.png -o ./out.txt -g 255 -vd -s 100 100
	
clean:
	rm -f $(ODIR)/*.o $(TARGET)