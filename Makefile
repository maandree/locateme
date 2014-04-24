# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.


# The package path prefix, if you want to install to another root, set DESTDIR to that root
PREFIX ?= /usr
# The command path excluding prefix
BIN ?= /bin
# The resource path excluding prefix
DATA ?= /share
# The command path including prefix
BINDIR ?= $(PREFIX)$(BIN)
# The resource path including prefix
DATADIR ?= $(PREFIX)$(DATA)
# The generic documentation path including prefix
DOCDIR ?= $(DATADIR)/doc
# The info manual documentation path including prefix
INFODIR ?= $(DATADIR)/info
# The license base path including prefix
LICENSEDIR ?= $(DATADIR)/licenses

# The name of the command as it should be installed
COMMAND ?= locateme
# The name of the package as it should be installed
PKGNAME ?= locateme

# Optimisation settings for C code compilation
OPTIMISE ?= -Og -g
# Warnings settings for C code compilation
WARN = -Wall -Wextra -Wdouble-promotion -Wformat=2 -Winit-self -Wmissing-include-dirs            \
       -Wtrampolines -Wfloat-equal -Wshadow -Wmissing-prototypes -Wmissing-declarations          \
       -Wredundant-decls -Wnested-externs -Winline -Wno-variadic-macros -Wsync-nand              \
       -Wunsafe-loop-optimizations -Wcast-align -Wstrict-overflow -Wdeclaration-after-statement  \
       -Wundef -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wlogical-op -Waggregate-return   \
       -Wstrict-prototypes -Wold-style-definition -Wpacked -Wvector-operation-performance        \
       -Wunsuffixed-float-constants -Wsuggest-attribute=const -Wsuggest-attribute=noreturn       \
       -Wsuggest-attribute=pure -Wsuggest-attribute=format -Wnormalized=nfkc -Wconversion        \
       -fstrict-aliasing -fstrict-overflow -fipa-pure-const -ftree-vrp -fstack-usage             \
       -funsafe-loop-optimizations
# excluded: -pedantic
# The C standard for C code compilation
STD = c99
FLAGS = -std=$(STD) $(WARN) $(OPTIMISE) $(CFLAGS) $(LDFLAGS) $(CPPFLAGS)


# Build rules

.PHONY: all
all: bin/locateme


bin/locateme: $(foreach F,locateme common fallback conffile coordinator spawn,obj/$(F).o)
	@mkdir -p bin
	$(CC) $(FLAGS) -o $@ $^

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(FLAGS) -c -o $@ $^

obj/locateme.c: $(foreach F,conffile coordinator,src/$(F).h)
obj/fallback.c: $(foreach F,common,src/$(F).h)
obj/conffile.c: $(foreach F,common,src/$(F).h)
obj/common.c: $(foreach F,coordinator,src/$(F).h)
obj/coordinator.c: $(foreach F,fallback spawn,src/$(F).h)
obj/spawn.c: $(foreach F,common,src/$(F).h)


# Clean rules

.PHONY: all
clean:
	-rm -r bin obj

