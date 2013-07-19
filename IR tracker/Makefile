# --------------------------------------------------------
# Custom M2 Makefile
# written by: Jonathan Bohren & Jonathan Fiene
# updated: June 16, 2012
# --------------------------------------------------------

# --------------------------------------------------------
# if you write separate C files to include in main,
# add their .o targets to the CHILDREN line below
# (e.g. "CHILDREN = myfile.o")
#
# to include code supplied by maevarm, add a .o target
# tag to the parents line (e.g. "PARENTS = "m_bus.o")
# --------------------------------------------------------
MAIN       = brain.o 
CHILDREN   = 
PARENTS    = 

# --------------------------------------------------------
# if you want to use one of our pre-compiled libraries,
# add it to the line below (e.g. "LIBRARIES = libsaast.a")
# --------------------------------------------------------

LIBRARIES = ../libsaast.a

# --------------------------------------------------------
# you shouldn't change anything below here,
# unless you really know what you're doing
# --------------------------------------------------------
DEVICE     = atmega32u4
CLOCK      = 16000000

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets: 
all:	brain.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

install: flash 

flash: all
	dfu-programmer atmega32u4 erase
	dfu-programmer atmega32u4 flash brain.hex

clean:
	rm -f brain.hex brain.elf $(MAIN) $(CHILDREN)

# file targets:
brain.elf: $(MAIN) $(CHILDREN) $(PARENTS)
	$(COMPILE) -o brain.elf $(MAIN) $(CHILDREN) $(PARENTS) $(LIBRARIES)

brain.hex: brain.elf
	rm -f brain.hex
	avr-objcopy -j .text -j .data -O ihex brain.elf brain.hex

# Targets for code debugging and analysis:
disasm:	brain.elf
	avr-objdump -d brain.elf

cpp:
	$(COMPILE) -E brain.c
