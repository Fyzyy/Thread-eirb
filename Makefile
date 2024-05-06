CC = gcc
CFLAGS = -Wall -Wextra -g -I$(SRCDIR)
PTHREAD_FLAGS = -pthread -DUSE_PTHREAD

SRCDIR = src
BUILDDIR = build
EXDIR = examples
TSTDIR = tst
INSTALLDIR = install

NB_THREADS_TO_CREATE=10
NB_THREADS_TO_YIELD=10

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o))
EXAMPLES := $(wildcard $(EXDIR)/*.c)
TST := $(wildcard $(TSTDIR)/*.c)
# TST := $(TSTDIR)/01-main.c\
# 	$(TSTDIR)/02-switch.c\
# 	$(TSTDIR)/03-equity.c\
# 	$(TSTDIR)/11-join.c\
# 	$(TSTDIR)/12-join-main.c\
# 	$(TSTDIR)/21-create-many.c\
# 	$(TSTDIR)/22-create-many-recursive.c\
# 	$(TSTDIR)/23-create-many-once.c\
# 	$(TSTDIR)/31-switch-many.c\
# 	$(TSTDIR)/32-switch-many-join.c\
# 	$(TSTDIR)/33-switch-many-cascade.c\
# 	$(TSTDIR)/51-fibonacci.c\
# 	$(TSTDIR)/61-mutex.c\
# 	$(TSTDIR)/62-mutex.c\
# 	$(TSTDIR)/63-mutex-equity.c\
# 	$(TSTDIR)/64-mutex-join.c\
# 	$(TSTDIR)/71-preemption.c\
# 	$(TSTDIR)/81-deadlock.c

EXECUTABLES_EXAMPLES := $(patsubst $(EXDIR)/%,$(BUILDDIR)/%,$(EXAMPLES:.c=))
EXECUTABLES_TST := $(patsubst $(TSTDIR)/%,$(BUILDDIR)/%,$(TST:.c=))
EXECUTABLES_TST_PTHREAD := $(patsubst $(TSTDIR)/%,$(BUILDDIR)/%,$(TST:.c=-pthread))

.PHONY: all check valgrind pthread examples install clean

all: libthread.a $(EXECUTABLES_TST)

pthreads: libthread.a $(EXECUTABLES_TST_PTHREAD)

examples: $(EXECUTABLES_EXAMPLES)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

libthread.a: $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	ar rcs $(BUILDDIR)/$@ $^

# Test with libthread
$(BUILDDIR)/%: $(TSTDIR)/%.c libthread.a
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $< -L$(BUILDDIR) -lthread -o $@

# Test with pthread
$(BUILDDIR)/%-pthread: $(TSTDIR)/%.c libthread.a
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) $< -L$(BUILDDIR) -lthread -o $@

# Examples
$(BUILDDIR)/%: $(EXDIR)/%.c libthread.a
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS)  $< -L$(BUILDDIR) -lthread -o $@

check: $(EXECUTABLES_TST)
	./exec_test.sh;

valgrind: $(EXECUTABLES_TST)
	./exec_test_valgrind.sh

install: $(EXECUTABLES_TST) $(EXECUTABLES_TST_PTHREAD)
	@mkdir -p $(INSTALLDIR)/lib $(INSTALLDIR)/bin
	@mv $(BUILDDIR)/libthread.a $(INSTALLDIR)/lib
	@mv $(EXECUTABLES_TST) $(INSTALLDIR)/bin
	@mv $(EXECUTABLES_TST_PTHREAD) $(INSTALLDIR)/bin
	@rm -rf $(BUILDDIR)

# graphs: all pthreads
	# taskset -c 1 python3 plot.py

clean:
	@rm -rf $(BUILDDIR) $(INSTALLDIR)
