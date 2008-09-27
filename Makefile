CC := gcc
RM := rm -f

GOBJECT_CFLAGS := $(shell pkg-config --cflags gobject-2.0)
GOBJECT_LIBS := $(shell pkg-config --libs gobject-2.0)

CFLAGS += -Wall

objects := pecan_session.o

sources := $(objects:.o=.c)
deps := $(objects:.o=.d)

.PHONY: clean

# Default target:
all:

version := 0.1

# from Lauri Leukkunen's build system
ifdef V
Q = 
P = @printf "" # <- space before hash is important!!!
else
P = @printf "[%s] $@\n" # <- space before hash is important!!!
Q = @
endif

target = libmsn-pecan.a
$(target): $(objects)
$(target): CFLAGS := $(CFLAGS) $(GOBJECT_CFLAGS) -D VERSION='"$(version)"'
$(target): LIBS := $(GOBJECT_LIBS)
all: $(target)

%.a::
	$(P)ARCHIVE
	$(Q)$(AR) rcs $@ $^

%.o:: %.c
	$(P)CC
	$(Q)$(CC) $(CFLAGS) -MMD -o $@ -c $<

clean:
	@printf "[CLEAN]\n"
	$(Q)find -name '.*.d' -delete
	$(Q)$(RM) $(target) $(objects) $(deps)

-include $(deps)
