CC := gcc
TARGET := imgConv
CFLAGS = -std=c99 -I $(IDIR) -Wall -lpng -lm

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

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(TARGET)
