CC = gcc
CFLAGS = -Wall -Wextra -I$(SRCDIR)
LDFLAGS = -L$(BUILDDIR)

SRCDIR = src
BUILDDIR = build
EXDIR = examples
TSTDIR = tst

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o))
EXAMPLES := $(wildcard $(EXDIR)/*.c)
TST := $(wildcard $(TSTDIR)/*.c)

EXECUTABLES_EXAMPLES := $(patsubst $(EXDIR)/%,$(BUILDDIR)/%,$(EXAMPLES:.c=))
EXECUTABLES_TST := $(patsubst $(TSTDIR)/%,$(BUILDDIR)/%,$(TST:.c=))

.PHONY: all check valgrind pthread install clean run

all: LDLIBS = -L$(BUILDDIR) -lthread  
all: install

pthread: CFLAGS += -pthread -DUSE_PTHREAD
pthread: $(EXECUTABLES_TST)

examples: $(EXECUTABLES_EXAMPLES)


exec: libthread.so $(EXECUTABLES_TST)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -fPIC -o $@ $<

libthread.so: $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CC) -shared -o $(BUILDDIR)/$@ $^

$(BUILDDIR)/%: $(TSTDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $< -o $@

$(BUILDDIR)/%: $(EXDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $< -o $@

check: $(EXECUTABLES_TST)
	@for exe in $(EXECUTABLES_TST); do \
	    echo $$exe; \
	    $$exe; \
	done

valgrind: $(EXECUTABLES_TST)
	@for exe in $(EXECUTABLES_TST); do \
		valgrind --leak-check=full --show-reachable=yes --track-origins=yes $$exe; \
	done

install: libthread.so $(EXECUTABLES_TST)
	@mkdir -p $(INSTALLDIR)/lib $(INSTALLDIR)/bin
	@cp $(BUILDDIR)/$< $(INSTALLDIR)/lib
	@cp $(EXECUTABLES_TST) $(INSTALLDIR)/bin

clean:
	@rm -rf $(BUILDDIR) $(INSTALLDIR)

run: export LD_LIBRARY_PATH=./$(INSTALLDIR)/lib
