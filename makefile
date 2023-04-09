# Makefile

PROJECT = enigma-chess
CC = emcc
CFLAGS = -s "EXPORTED_FUNCTIONS=['_add','_multiply','_subtract']" -s "EXPORTED_RUNTIME_METHODS=['cwrap']"
SRCDIR = src
INCDIR = include
BUILDDIR = build

SOURCES := $(wildcard $(SRCDIR)/*.cc)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS := $(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(SOURCES))
OUTPUT := $(BUILDDIR)/enigma-chess.js

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc $(INCLUDES)
	mkdir -p $(BUILDDIR)
	$(CC) -c $< -o $@ -I $(INCDIR)

clean:
	rm -rf $(BUILDDIR)
