# -----------------------------------
#             IMPORTANT
# -----------------------------------
# Customize this to fit your hardware
# -----------------------------------
# The default is sensible for TI Launchpad
# board that comes pre-populated with M430G2553
MCU?=msp430g2553

# Use the toolchain provided by gcc-m430 package
CC=msp430-gcc

# Explicitly specify the MCU we're using so that
# appropriate include files and linker scripts
# are pulled in
LDFLAGS+=-mmcu=$(MCU)

# Enable all warnings by default
CFLAGS=-Wall

# Treat each c-file as a standalone example
EXAMPLES=$(patsubst %.c,%.elf,$(wildcard *.c))
$(EXAMPLES): %.elf: %.c
	$(LINK.c) $^ $(OUTPUT_OPTION)

# Build all of the examples
.PHONY: all
all: $(EXAMPLES)

# Clean all of the elf files
.PHONY: clean
clean:
	rm -f $(EXAMPLES)

# Don't apply any implicit rules from make
.SUFFIXES:
