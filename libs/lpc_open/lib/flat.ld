INCLUDE libs.ld

MEMORY
{
  FLATRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

ENTRY(Reset_Handler)

SECTIONS
{
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        KEEP(*(.isr_vendor_vector))

        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */


        *(.after_vectors*)
    } >FLATRAM
    . = ALIGN(4);

   .preinit_array     :
   {
     PROVIDE_HIDDEN (__preinit_array_start = .);
     KEEP (*(.preinit_array*))
     PROVIDE_HIDDEN (__preinit_array_end = .);
   } >FLATRAM
   
   .init_array :
   {
     PROVIDE_HIDDEN (__init_array_start = .);
     KEEP (*(SORT(.init_array.*)))
     KEEP (*(.init_array*))
     PROVIDE_HIDDEN (__init_array_end = .);
   } >FLATRAM
  
    . = ALIGN(4);

    .text : ALIGN(4)
    {
         *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);

    } > FLATRAM

    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this.
     */
    .ARM.extab : ALIGN(4)
    {
       *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLATRAM
    __exidx_start = .;

    .ARM.exidx : ALIGN(4)
    {
       *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLATRAM
    __exidx_end = .;

    _etext = .;


    /* MAIN DATA SECTION */


    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > FLATRAM


   /* Main DATA section (RamLoc32) */
   .data : ALIGN(4)
   {
      FILL(0xff)
      _data = . ;
      *(vtable)
      *(.ramfunc*)
      *(.data*)
      . = ALIGN(4) ;
      _edata = . ;
   } > FLATRAM

    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > FLATRAM

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*)
         . = ALIGN(4) ;
        _end_noinit = .;
    } > FLATRAM

    PROVIDE(_pvHeapStart = .);
    PROVIDE(_vStackTop = ORIGIN(FLATRAM) + LENGTH(FLATRAM) );
    PROVIDE(__valid_user_code_checksum = 0 -
         (_vStackTop
         + (Reset_Handler + 1)
         + (NMI_Handler + 1)
         + (HardFault_Handler + 1)
         + (( DEFINED(MemManage_Handler) ? MemManage_Handler : 0 ) + 1)   /* MemManage_Handler may not be defined */
         + (( DEFINED(BusFault_Handler) ? BusFault_Handler : 0 ) + 1)     /* BusFault_Handler may not be defined */
         + (( DEFINED(UsageFault_Handler) ? UsageFault_Handler : 0 ) + 1) /* UsageFault_Handler may not be defined */
         ) );
}
