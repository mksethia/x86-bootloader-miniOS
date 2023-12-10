mkdir -p temp

# Compile and link kernel and kernel entry
nasm "boot/boot.asm" -f bin -o "temp/boot.bin"

nasm "kernel/kernel_entry.asm" -f elf -o "temp/kernel_entry.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/kernel.cpp" -o "temp/kernel.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/idt/idt.cpp" -o "temp/idt.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/idt/isr.cpp" -o "temp/isr.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/idt/irq.cpp" -o "temp/irq.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/dvr/pit.cpp" -o "temp/pit.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/dvr/keyboard.cpp" -o "temp/keyboard.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/dvr/vga.cpp" -o "temp/vga.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/dvr/mouse.cpp" -o "temp/mouse.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/ui/gui.cpp" -o "temp/gui.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/mem/page.cpp" -o "temp/page.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/mem/memmgr.cpp" -o "temp/memmgr.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/programs/program1.cpp" -o "temp/program1.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/programs/program2.cpp" -o "temp/program2.o"
i386-elf-g++ -ffreestanding -m32 -g -c "kernel/programs/program3.cpp" -o "temp/program3.o"



i386-elf-ld -o "temp/kernelentry.bin" -Ttext 1000 "temp/kernel_entry.o" "temp/kernel.o" "temp/idt.o" "temp/isr.o" "temp/irq.o" "temp/pit.o" "temp/keyboard.o" "temp/vga.o" "temp/mouse.o" "temp/gui.o" "temp/page.o" "temp/memmgr.o" "temp/program1.o" "temp/program2.o" "temp/program3.o" --oformat binary #&>/dev/null


# Create padding file, assemble, and concatenate everything together
echo "times 25600 db 0x00" > temp/padding.asm
nasm "temp/padding.asm" -f bin -o "temp/padding.bin"

cat "temp/boot.bin" "temp/kernelentry.bin" "temp/padding.bin"  > "full_kernel.bin"

# Launch QEMU
qemu-system-i386 -cpu EPYC-v1 -m 4096M -hda "full_kernel.bin" -vga std -monitor stdio &>/dev/null

# Debugged QEMU
#qemu-system-i386 -s -S -cpu EPYC-v1 -m 4096M -hda "full_kernel.bin" -vga std -monitor stdio &>/dev/null


# add a break at 0x7c00!, or 7c21, or 7c8d, 7cb8 (5x), 7cf4




# 21
# future
# ye
# kc
# post
# phareel
# i know nigo!
# pop smoke
# don toliver
# luv new tape
# biggie
# pac
# kendrick
# rich 





# programs 1 - 3
# programs 1 - 3 are written in pre-existing cpp files that are compiled into os
# programs 1 - 3 run one at a time but can save and restore state when switching. 
# from HOMEPAGE: start / continue / exit programs 1-3
# from PROGRAMPAGE: back to home / quit program / start programs 1-3 / continue programs 1-3

# register states maybe not saved, but page directory IS swapped. 
# kernel code is linked across kernels, free space is constant 
# newpage() will need to be called for new memory


# program running: pd saved. 
# program quit: pd deleted. 

