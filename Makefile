CC = gcc
CFLAGS = -Wall -Wextra -Isrc/

SRCDIR = src
BUILDDIR = build
EXDIR = examples
INSTALLDIR = install

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o))
EXAMPLES := $(wildcard $(EXDIR)/*.c)
EXECUTABLES := $(patsubst $(EXDIR)/%,$(BUILDDIR)/%,$(EXAMPLES:.c=))

.PHONY: all clean

all: $(EXECUTABLES)

$(BUILDDIR)/%: $(EXDIR)/%.c $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -rf $(BUILDDIR)
