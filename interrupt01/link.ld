ENTRY(_start) /* Where the program starts */

/* SECTIONS describs each part of memory in main.o */
SECTIONS {
    . = 0x80000;         /* Kernel load address */
    .text : { *(.text*) } /* Group of text from compiled object. Example: name of a function, add() */
    .data : { *(.data*) } /* Group of initialized variable. Example int x = 5; */
    .bss  : { *(.bss*)  } /* Group of not initilized variable. Example: int y; */
}