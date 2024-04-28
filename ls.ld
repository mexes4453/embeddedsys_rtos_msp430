ENTRY(handler_reset)
MEMORY
{
	RAM(RWX)      : ORIGIN=0x2400, LENGTH=0x2000  /* END 0x43FF,  bytes: 8192  */  
	ROM(RX)       : ORIGIN=0x4400, LENGTH=0x10000 /* END 0x143FF, bytes: 65536 */  
	ISR_VEC_RESET : ORIGIN=0xFFFE, LENGTH=0x02    /* END 0xFFFF,  bytes: 2     */
}

SECTION
{
    .isr_reset: { *(.isr_reset) }>ISR_VEC_RESET    
    .text:
    {
	   _text_start_ = .;
	   *(.isr_vec)
	   *(.text)         /* Merge all text section of all input file          */
	   . = ALIGN(2);    /* Align the location counter to an aligned address  */
	   _text_end_ = .;
    }>ROM

    .data:
    {
	   _stack_ = . + 0x0800;  /* 2kb of stack on RAM */
	   _data_start_ = .;
	   *(.data)         /* Merge all text section of all input file          */
	   _data_end_ = .;
	   . = ALIGN(2);    /* Align the location counter to an aligned address  */
    }>RAM AT>ROM

    .bss:
    {
	   _bss_start_ = .;
	   *(.bss)         /* Merge all text section of all input file          */
	   _bss_end_ = .;
	   . = ALIGN(2);    /* Align the location counter to an aligned address  */
    }>RAM
    PROVIDE(_bss_size_ = SIZEOF(.bss))

    .rodata:
    {
	   _rodata_start_ = .;
	   *(.rodata)         /* Merge all text section of all input file          */
	   _rodata_end_ = .;
	   . = ALIGN(2);    /* Align the location counter to an aligned address  */
    }>ROM
}

