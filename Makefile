CC = gcc
CFLAGS = -Wall -Wextra -I$(SRCDIR)
PTHREAD_FLAGS = -pthread -DUSE_PTHREAD

SRCDIR = src
BUILDDIR = build
EXDIR = examples
TSTDIR = tst
INSTALLDIR = install

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o))
EXAMPLES := $(wildcard $(EXDIR)/*.c)
TST := $(wildcard $(TSTDIR)/*.c)
TST_PTHREAD := $(TST:.c=-pthread)

EXECUTABLES_EXAMPLES := $(patsubst $(EXDIR)/%,$(BUILDDIR)/%,$(EXAMPLES:.c=))
EXECUTABLES_TST := $(patsubst $(TSTDIR)/%,$(BUILDDIR)/%,$(TST:.c=))
EXECUTABLES_TST_PTHREAD := $(patsubst $(TSTDIR)/%,$(BUILDDIR)/%,$(TST:.c=-pthread))

.PHONY: all check valgrind pthread examples install clean

all: libthread.a $(EXECUTABLES_TST)

pthread: libthread.a $(EXECUTABLES_TST_PTHREAD)

examples: $(EXECUTABLES_EXAMPLES)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

libthread.a: $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	ar rcs $(BUILDDIR)/$@ $^

$(BUILDDIR)/%: $(TSTDIR)/%.c libthread.a
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $< -L$(BUILDDIR) -lthread -o $@

$(BUILDDIR)/%-pthread: $(TSTDIR)/%.c libthread.a
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) $< -L$(BUILDDIR) -lthread -o $@

$(BUILDDIR)/%: $(EXDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS)  $< -L$(BUILDDIR) -lthread -o $@

check: $(EXECUTABLES_TST)
	@for exe in $(EXECUTABLES_TST); do \
		echo $$exe; \
		$$exe; \
	done

valgrind: $(EXECUTABLES_TST)
	@for exe in $(EXECUTABLES_TST); do \
		valgrind --leak-check=full --show-reachable=yes --track-origins=yes $$exe; \
	done

install: $(EXECUTABLES_TST) $(EXECUTABLES_TST_PTHREAD)
	@mkdir -p $(INSTALLDIR)/lib $(INSTALLDIR)/bin
	@mv $(BUILDDIR)/libthread.a $(INSTALLDIR)/lib
	@mv $(EXECUTABLES_TST) $(INSTALLDIR)/bin
	@mv $(EXECUTABLES_TST_PTHREAD) $(INSTALLDIR)/bin
	@rm -rf $(BUILDDIR)

clean:
	@rm -rf $(BUILDDIR) $(INSTALLDIR)
