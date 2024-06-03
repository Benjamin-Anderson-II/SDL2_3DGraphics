# Comprehensive Makefile
# This will automatically compile any .c files in the current directory.
# Some of the syntax was adapted from:
# https://gist.github.com/Wenchy/64db1636845a3da0c4c7
# http://nuclear.mutantstargoat.com/articles/make/

CC = gcc

CFLAGS := -I$(INCDIR)

BINDIR = .
exe_file = $(BINDIR)/$(shell basename "${PWD}")

# Handle debug case
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS += -g -Wall
else
	CFLAGS += -DNDEBUG -O3
endif

SRCDIR = src
BUILDDIR = build
SRCEXT = c
SOURCES = $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEP = $(OBJECTS:.o=.d)
LIB = -lm -lSDL2

all: $(exe_file)

$(exe_file): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) -o $(exe_file) $^ $(LIB) $(LDFLAGS)

$(BUILDDIR)/%.d: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@$(CC) $(INC) $< -MM -MT $(@:.d=.o) >$@

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(exe_file)

-include $(DEP)

