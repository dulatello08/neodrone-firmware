/* Linker script for ATSAMD21 */

/* OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm") */
/* OUTPUT_ARCH(arm) */

MEMORY
{
    /* 
     * SAMD21G18A: FLASH = 256K, RAM = 32K
     * SAMD21G17A: FLASH = 128K, RAM = 16K
     * SAMD21G16A: FLASH = 64K,  RAM = 8K
     * SAMD21G15A: FLASH = 32K,  RAM = 4K
     */
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K 
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 32K 
}

/* Section Definitions */
SECTIONS
{
    .text :
    {
        _sfixed = .;
        KEEP(*(.vectors .vectors.*))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)

        /* Support C++ constructors, etc. */
        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        . = ALIGN(4);
        KEEP(*(.fini))

        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

        . = ALIGN(4);
        _efixed = .;            /* End of text section */
    } > FLASH

    .ARM.exidx : {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH

    .relocate : AT (_efixed)
    {
        . = ALIGN(4);
        _srelocate = .;
        *(.ramfunc .ramfunc.*);
        *(.data .data.*);
        . = ALIGN(4);
        _erelocate = .;
    } > RAM

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _szero = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ezero = .;
    } > RAM

    /* Stack definition */
    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        _sstack = .;
        . = . + 0x1000; /* Min stack size */
        . = ALIGN(8);
        _estack = .;
    } > RAM

    _etext = _efixed;
}
