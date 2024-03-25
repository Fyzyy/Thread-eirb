CC = gcc
CFLAGS = -Wall -Wextra -I$(SRCDIR) -fPIC
LDFLAGS = -shared

SRCDIR = src
BUILDDIR = build
EXDIR = examples
INSTALLDIR = install
TSTDIR = tst

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o))
EXAMPLES := $(wildcard $(EXDIR)/*.c)
TST := $(wildcard $(TSTDIR)/*.c)

EXECUTABLES_EXAMPLES := $(patsubst $(EXDIR)/%,$(BUILDDIR)/%,$(EXAMPLES:.c=))
EXECUTABLES_TST := $(patsubst $(TSTDIR)/%,$(BUILDDIR)/%,$(TST:.c=))

.PHONY: all check valgrind pthread install clean

all: exec 

pthread: CFLAGS += -pthread -DUSE_PTHREAD
pthread: exec

examples: $(EXECUTABLES_EXAMPLES)

exec: $(EXECUTABLES_TST)

$(BUILDDIR)/%: $(TSTDIR)/%.c $(BUILDDIR)/libthread.so
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -L$(BUILDDIR) $< -lthread $(LDFLAGS) -o $@

$(BUILDDIR)/%: $(EXDIR)/%.c $(BUILDDIR)/libthread.so
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -L$(BUILDDIR) $< -lthread $(LDFLAGS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/libthread.so: $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

check: $(EXECUTABLES_TST)
	@for exe in $(EXECUTABLES_TST); do \
		$$exe; \
	done

valgrind: $(EXECUTABLES_TST)
	@for exe in $(EXECUTABLES_TST); do \
		valgrind --leak-check=full --show-reachable=yes --track-origins=yes $$exe; \
	done

install: $(BUILDDIR)/libthread.so $(EXECUTABLES_TST)
	@mkdir -p $(INSTALLDIR)/lib $(INSTALLDIR)/bin
	@cp $< $(INSTALLDIR)/lib
	@cp $(EXECUTABLES_TST) $(INSTALLDIR)/bin

clean:
	@rm -rf $(BUILDDIR) $(INSTALLDIR)
