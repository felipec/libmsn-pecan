CC := gcc

LIBMSN_PECAN_CFLAGS := -I..
LIBMSN_PECAN_LIBS := -lmsn-pecan -L..

GOBJECT_CFLAGS := $(shell pkg-config --cflags gobject-2.0)
GOBJECT_LIBS := $(shell pkg-config --libs gobject-2.0)

CFLAGS += -Wall

override CFLAGS += -I.

.PHONY: clean

test_objects := test.o

objects += $(test_objects)
deps += $(test_objects:.o=.d)
bins += test

all: $(bins)

# from Lauri Leukkunen's build system
ifdef V
Q = 
P = @printf "" # <- space before hash is important!!!
else
P = @printf "[%s] $@\n" # <- space before hash is important!!!
Q = @
endif

test: $(test_objects)
test: CFLAGS := $(CFLAGS) $(GOBJECT_CFLAGS) $(LIBMSN_PECAN_CFLAGS)
test: LIBS := $(GOBJECT_LIBS) $(LIBMSN_PECAN_LIBS)

$(bins):
	$(P)LINK
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o:: %.c
	$(P)CC
	$(Q)$(CC) $(CFLAGS) -MMD -o $@ -c $<

clean:
	$(Q)rm -vf $(bins) $(objects) $(deps)

-include $(deps)
