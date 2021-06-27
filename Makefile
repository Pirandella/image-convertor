CC := gcc
TARGET := qrs
CFLAGS = -std=c99 -I $(IDIR) -Wall -Wextra

ODIR := ./obj
SDIR := ./src
IDIR := ./inc

_DEPS := 
DEPS := $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ := main.o
OBJ := $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(TARGET)
