CC = arm-none-eabi-gcc
Controller = cortex-m4
Flag = -c -mcpu=$(Controller) -nostdlib -mthumb  -mfloat-abi=soft  -Wall 

Flag_Linker = -mcpu=$(Controller) -nostdlib  -mthumb -mfloat-abi=soft  -T LinkerScript.ld -Wl,-Map=final.map

all: main.o blink.o startup.o final.elf

blink.o:blink.c
	$(CC) $(Flag) blink.c -o blink.o

main.o:main.c
	$(CC) $(Flag) main.c -o main.o

startup.o:startup.c
	$(CC) $(Flag) $^ -o $@

final.elf: blink.o main.o startup.o
	$(CC) $(Flag_Linker) $^ -o $@

clean:
	rm -rf *.o *.elf *.map

load:
	openocd  -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f board/stm32f4discovery.cfg 