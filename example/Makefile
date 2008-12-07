CC := gcc

LIBMSN_PECAN_CFLAGS := -I..
LIBMSN_PECAN_LIBS := -lmsn-pecan -L..

GOBJECT_CFLAGS := $(shell pkg-config --cflags gobject-2.0)
GOBJECT_LIBS := $(shell pkg-config --libs gobject-2.0)

CFLAGS += -Wall

override CFLAGS += -I.

.PHONY: clean

all:

test_objects := test.o

test: $(test_objects)
test: CFLAGS := $(CFLAGS) $(GOBJECT_CFLAGS) $(LIBMSN_PECAN_CFLAGS)
test: LIBS := $(GOBJECT_LIBS) $(LIBMSN_PECAN_LIBS)

objects += $(test_objects)
deps += $(test_objects:.o=.d)
bins += test

ssl_objects := ssl.o

ssl: $(ssl_objects)
ssl: CFLAGS := $(CFLAGS) $(GOBJECT_CFLAGS) $(LIBMSN_PECAN_CFLAGS)
ssl: LIBS := $(GOBJECT_LIBS) $(LIBMSN_PECAN_LIBS)

objects += $(ssl_objects)
deps += $(ssl_objects:.o=.d)
bins += ssl

all: $(bins)

# from Lauri Leukkunen's build system
ifdef V
Q = 
P = @printf "" # <- space before hash is important!!!
else
P = @printf "[%s] $@\n" # <- space before hash is important!!!
Q = @
endif

$(bins):
	$(P)LINK
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o:: %.c
	$(P)CC
	$(Q)$(CC) $(CFLAGS) -MMD -o $@ -c $<

clean:
	$(Q)rm -vf $(bins) $(objects) $(deps)

-include $(deps)
