
build:
	/usr/local/i386elfgcc/bin/i386-elf-g++ -ffreestanding  -m32 -g -c *.cpp  -Wno-write-strings
	nasm -f elf32 entry.asm
	/usr/local/i386elfgcc/bin/i386-elf-ld -o "../../root/APPS/TEST2"  -Tlink.ld *.o --oformat binary
	rm *.o


 
