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
MAIN       = controller.o 
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
all:	controller.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

install: flash 

flash: all
	dfu-programmer atmega32u4 erase
	dfu-programmer atmega32u4 flash controller.hex

clean:
	rm -f controller.hex controller.elf $(MAIN) $(CHILDREN)

# file targets:
controller.elf: $(MAIN) $(CHILDREN) $(PARENTS)
	$(COMPILE) -o controller.elf $(MAIN) $(CHILDREN) $(PARENTS) $(LIBRARIES)

controller.hex: controller.elf
	rm -f controller.hex
	avr-objcopy -j .text -j .data -O ihex controller.elf controller.hex

# Targets for code debugging and analysis:
disasm:	controller.elf
	avr-objdump -d controller.elf

cpp:
	$(COMPILE) -E controller.c
