CC ?= gcc
INCLUDE ?= -I$(TAPDIR)
CFLAGS ?= -m64 -W -Wall -std=c11 -pedantic -O2 -s
LIBS = $(TAPDIR)/libtap.so
PYTHON ?= python3
srcdir = $(CURDIR)/t

TESTS = $(srcdir)/test_api_base

all: test

test: $(TESTS)
	$(PYTHON) testrunner -t $(srcdir)

$(TESTS): $(srcdir)/%: $(srcdir)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) $< -o $@

dist:
	zip -r -9 python-testrunner *

clean:
	-rm -f python-testrunner.zip $(TESTS) $(srcdir)/*.got
