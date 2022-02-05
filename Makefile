AVRDIR = /usr/avr
DEVICE = /dev/ttyUSB0
FWLIST = grit bernoullator two-op dw8000

.PHONY: all clean $(FWLIST)
.SECONDARY: # this turns off stupid autodeletes

all:
	@echo "select firmware: $(FWLIST)"
clean:
	rm -rf build fw
%/:
	mkdir -p $@

disassemble: $(FWLIST:%=disasm/%.lst)
disasm/%.lst: build/%.elf | disasm/
	avr-objcopy -S $< > $@

build/%.o: src/%.c src/digenerator.h | build/
	avr-gcc -c -std=gnu99 -Os -Wall         \
	    -ffunction-sections -fdata-sections \
		-mmcu=atmega328p -DF_CPU=16000000   \
		-I$(AVRDIR)/include -o $@ $<

build/%.elf: build/%.o build/digenerator.o
	avr-gcc -ffunction-sections -fdata-sections \
		-Os -mmcu=atmega328p -Wl,--gc-sections -o $@ $^

fw/%.hex: build/%.elf | fw/
	avr-objcopy -O ihex -R .eeprom $< $@

$(FWLIST): %: fw/%.hex
	@echo "done"
#	avrdude -p atmega328p -c arduino -b 57600 \
#		-P $(DEVICE) -D -U flash:w:$<:i 
