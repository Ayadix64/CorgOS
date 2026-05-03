export PATH=$PATH:/usr/local/i386elfgcc/bin
rm Binaries/*.*
set -e


nasm "boot/boot.asm" -f bin -o "Binaries/boot.bin"
nasm "kernel/kernel_entry.asm" -f elf -o "Binaries/kernel_entry.o"
nasm "idt/isr.asm" -f elf -o   "Binaries/isr.o"
nasm "gdt/gdt.asm" -f elf32 -o "Binaries/gdt_loader.o"
#nasm "cpu/smp.asm" -f elf -o "Binaries/smpb.o"
cd Binaries
#i386-elf-gcc  -ffreestanding -m16 -g -c ../boot/stage2.c  -lgcc

i386-elf-g++  -ffreestanding -m32 -g -c ../*/*.cpp  -Wno-write-strings
i386-elf-g++  -ffreestanding -m32 -g -c ../*/*/*.cpp  -Wno-write-strings
cd ..
nasm "boot/zeroes.asm" -f bin -o "Binaries/zeroes.bin"
#i386-elf-ld -o "Binaries/full_kernel.bin"  -Tlink.ld "Binaries/kernel_entry.o" "Binaries/kernel.o" "Binaries/idt.o" "Binaries/CPU.o" "Binaries/isr.o" \
#                                                     "Binaries/timer.o" "Binaries/RTC.o" "Binaries/paging.o" "Binaries/mem.o" --oformat binary -L /usr/lib/gcc/i386-elf/14.2.0/ -lgcc
#
i386-elf-ld -o "Binaries/full_kernel.bin"  -Tlink.ld ./Binaries/*.o --oformat binary -L /usr/local/i386elfgcc/lib/gcc/i386-elf/14.2.0 -lgcc -lgcov


cat "Binaries/boot.bin" "Binaries/full_kernel.bin" "Binaries/zeroes.bin"  > "Binaries/OS.bin"
dd if="Binaries/OS.bin" of="./OS.img" bs=512 count=255 conv=notrunc

