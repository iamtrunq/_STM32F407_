

/*Entry Point*/
ENTRY(Reset_Handler)


MEMORY
{
    /* See memory mapping in Datasheet to configure */
    FLASH(rx) :ORIGIN =0x08000000, LENGTH =1024K
    SRAM(rwx) :ORIGIN =0x20000000, LENGTH =128K
    
}

SECTIONS
{
    /*Text section*/
    .text :
    {
        _stext_ = .;
        *(.ISR)
        *(.text)
        *(.rodata)
        _etext_ = .;
    }>FLASH

    /*Data section*/
    _lddata = LOADADDR(.data); /*Save start address of data section in _lddata sybmbol*/
    .data :
    {
        _sdata_ = .;
        *(.data)
        . = ALIGN(4);
        _edata_ = .;
    }>SRAM AT>FLASH

    /*bss section*/
    .bss :
    {
        _sbss_ = .;
        *(.bss)
        _ebss_ = .;
    }>SRAM

}