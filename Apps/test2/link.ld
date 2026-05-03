ENTRY(_start)
SECTIONS {
    
    . = 0x100000;

    /* Make sure the entry point is the very first thing that is placed in the binary */
    .entry : { *(.entry) }
    .text : { *(.text) }

    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : {
        *(COMMON)
        *(.bss)
        
    }
      
}
