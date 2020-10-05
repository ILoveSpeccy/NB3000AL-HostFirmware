ENTRY(_start)

SECTIONS
{
   . = 0x00000000;

   .text :
   {
      _ftext = . ;
      PROVIDE (eprol = .);
      *(.text)
      *(.text.*)
   }

   PROVIDE (etext = .);
   _etext  =  .;

   . = .;

   .rodata :
   {
      *(.rdata)
      *(.rodata)
      *(.rodata.*)
   }

   _fdata = ALIGN(16);

   .data :
   {
      *(.data)
      *(.data.*)
   }

   . = ALIGN(8);
   _gp = . + 0x8000;
   __global = _gp;

   .lit8 :
   {
      *(.lit8)
   }

   .lit4 :
   {
      *(.lit4)
   }

   .sdata :
   {
      *(.sdata)
      *(.sdata.*)
   }

   . = ALIGN(4);
   PROVIDE (edata  =  .);
   _edata  =  .;
   _fbss = .;

   .sbss :
   {
      *(.sbss)
      *(.sbss.*)
      *(.scommon)
   }

   .bss :
   {
      _bss_start = . ;
      *(.bss)
      *(.bss.*)
      *(COMMON)
   }

   PROVIDE (end = .);
   _end = .;
}
