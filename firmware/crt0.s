   .text
   .align 2
   .global _start
   .ent _start

_start:
   .set noreorder

   la    $gp, _gp
   la    $4, _bss_start
   la    $5, _end
   la    $sp, 0x1FFC

_BSS_CLEAR:

   sw    $0, 0($4)
   slt   $3, $4, $5
   bnez  $3, _BSS_CLEAR
   addiu $4, $4, 4

   jal main
   nop

LOOP:
   j LOOP
   nop

.end _start
