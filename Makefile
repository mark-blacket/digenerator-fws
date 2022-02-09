AVRDIR = /usr/avr
DEVICE = /dev/ttyUSB0
FWLIST = bernoullator crossmod dw8000 repeater

.PHONY: all clean fullclean $(FWLIST)
.SECONDARY: # this turns off stupid autodeletes

all: $(FWLIST:%=fw/%.hex)
fw/%.hex: build/%.elf | fw/
	avr-objcopy -O ihex -R .eeprom $< $@

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

clean:
	rm -rf build disasm
fullclean:
	rm -rf build fw disasm
%/:
	mkdir -p $@
