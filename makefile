PROJECT = enigma-chess
CC = emcc
CFLAGS = -O3 --shell-file shell_minimal.html -s EXPORTED_FUNCTIONS="['_make_random_valid_move', '_malloc', '_free']" -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']" -s EXTRA_EXPORTED_RUNTIME_METHODS="['stringToUTF8', 'UTF8ToString']"
SRCDIR = src
INCDIR = include
BUILDDIR = build

SOURCES := $(wildcard $(SRCDIR)/*.cc)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS := $(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(SOURCES))
OUTPUT := $(BUILDDIR)/enigma-chess.html
WASM_OUTPUT := $(BUILDDIR)/enigma-chess.wasm
WAST_OUTPUT := $(BUILDDIR)/enigma-chess.wast
JS_OUTPUT := $(BUILDDIR)/enigma-chess.js

.PHONY: all clean deploy

all: $(OUTPUT) $(WASM_OUTPUT) $(WAST_OUTPUT) deploy

$(OUTPUT): $(OBJECTS)
	$(CC) $^ -g -o $@ $(CFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc $(INCLUDES)
	mkdir -p $(BUILDDIR)
	$(CC) -c $< -g -o $@ -I $(INCDIR)

$(WASM_OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -g -o $@ --no-entry

$(WAST_OUTPUT): $(WASM_OUTPUT)
	wasm2wat $< -o $@

deploy:
	mkdir -p site
	cp $(OUTPUT) site/
	cp $(WASM_OUTPUT) site/
	cp $(WAST_OUTPUT) site/
	cp $(JS_OUTPUT) site/

clean:
	rm -rf $(BUILDDIR)
