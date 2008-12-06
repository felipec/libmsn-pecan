CC := gcc
RM := rm -f

GOBJECT_CFLAGS := $(shell pkg-config --cflags gobject-2.0)
GOBJECT_LIBS := $(shell pkg-config --libs gobject-2.0)

CFLAGS += -Wall

override CFLAGS += -I.
override CFLAGS += -DPECAN_SOCKET -DPECAN_DEBUG

ifdef DEBUG_LEVEL
override CFLAGS += -D PECAN_LOG_LEVEL=PECAN_LOG_LEVEL_$(DEBUG_LEVEL)
else
override CFLAGS += -D PECAN_LOG_LEVEL=PECAN_LOG_LEVEL_INFO
endif

ifdef DEBUG
override CFLAGS += -ggdb
endif

objects := pecan_session.o \
	   pecan_ns.o \
	   pecan_printf.o \
	   pecan_log.o \
	   io/pecan_stream.o \
	   io/pecan_socket.o \
	   io/pecan_node.o \
	   io/pecan_cmd_node.o \
	   cmd/pecan_transaction.o \
	   cmd/pecan_command.o

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

target = libmsn-pecan.so
$(target): $(objects)
$(target): CFLAGS := $(CFLAGS) $(GOBJECT_CFLAGS) -D VERSION='"$(version)"'
$(target): LIBS := $(GOBJECT_LIBS)
all: $(target)

%.so::
	$(P)SHLIB
	$(Q)$(CC) $(LDFLAGS) -shared -o $@ $^ $(LIBS)

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
