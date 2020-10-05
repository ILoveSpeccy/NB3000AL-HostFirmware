#include "sx2.h"
#include "sx2_registers.h"
#include "hardware.h"
#include "descriptor.h"

#ifdef DEBUG
   #include <stdio.h>
#endif

// SX2 registers values
unsigned char regs[]       = {0x01, 0x02, 0x03, 0x04,
                              0x06, 0x07, 0x08, 0x09,
                              0x0A, 0x0B, 0x0C, 0x0D,
                              0x0E, 0x0F, 0x10, 0x11,
                              0x12, 0x13, 0x14, 0x15,
                              0x16, 0x17, 0x18, 0x19,
                              0x1A, 0x1B, 0x1C, 0x1D,
                              0x2E};

unsigned char regvalues[]  = {0b11001000, 0b00001000, 0b00000000, 0b00000000,
                              0b10100010, 0b11100010, 0b00100010, 0b01100010,
                              0b00000000, 0b01000000, 0b00000000, 0b01000000,
                              0b00000000, 0b01000000, 0b00000000, 0b01000000,
                              0b10000000, 0b01000000, 0b10000000, 0b01000000,
                              0b10000000, 0b01000000, 0b10000000, 0b01000000,
                              0b00000001, 0b00000001, 0b00000001, 0b00000001,
                              0b11111111};

// Selection FIFOADR2 FIFOADR1 FIFOADR0
// ------------------------------------
//   FIFO2      0        0        0
//   FIFO4      0        0        1
//   FIFO6      0        1        0
//   FIFO8      0        1        1
//   COMMAND    1        0        0
//   RESERVED   1        0        1
//   RESERVED   1        1        0
//   RESERVED   1        1        1
// ------------------------------------

void sx2_set_fifoadr(unsigned char addr)
{
   switch(addr)
   {
      case FIFO2:    A0_L; A1_L; A2_L; break;
      case FIFO4:    A0_H; A1_L; A2_L; break;
      case FIFO6:    A0_L; A1_H; A2_L; break;
      case FIFO8:    A0_H; A1_H; A2_L; break;
      case COMMAND:  A0_L; A1_L; A2_H; break;
   }
}

// Asynchronous Command/FIFO write
void sx2_low_level_write(unsigned char addr, unsigned char data)
{
   while(!statREADY);      // can write to sx2?
   sx2_set_fifoadr(addr);
   CS_L;
   SLWR_L;
   PPI_DIR = 1;
   PPI = data;
   SLWR_H;
   PPI_DIR = 0;
   CS_H;
}

// Asynchronous Command/FIFO read
unsigned char sx2_low_level_read(unsigned char addr)
{
   unsigned char data;
   sx2_set_fifoadr(addr);
   CS_L;
   SLOE_L;
   SLRD_L;
   data = PPI;
   SLRD_H;
   SLOE_H;
   CS_H;
	return data;
}

// Asynchronous Command/FIFO read
unsigned char sx2_fifo_read(void)
{
   unsigned char data;
   CS_L;
   SLOE_L;
   SLRD_L;
   data = PPI;
   SLRD_H;
   SLOE_H;
   CS_H;
	return data;
}

unsigned char sx2_read_reg(unsigned char reg)
{
   read_interrupt = 1;
	sx2_low_level_write(COMMAND, reg | RD_REQ);
   do {sx2_interrupt_handler();
   } while(read_interrupt);
	return sx2_low_level_read(COMMAND);
}

void sx2_write_reg(unsigned char reg, unsigned char data)
{
 	sx2_low_level_write(COMMAND, reg | WR_REQ);
	sx2_low_level_write(COMMAND, (data & 0xf0) >> 4);
	sx2_low_level_write(COMMAND, data & 0x0f);
}

void sx2_write_descriptor(void)
{
   unsigned short len,i;

   len = sizeof(descriptor);

   sx2_low_level_write(COMMAND, (0x30 | 0x80));
   sx2_low_level_write(COMMAND, (len & 0x00F0) >> 4);
   sx2_low_level_write(COMMAND, (len & 0x000F));
   sx2_low_level_write(COMMAND, (len & 0xF000) >> 12);
   sx2_low_level_write(COMMAND, (len & 0x0F00) >> 8);
   for (i = 0; i < len; i++)
   {
      sx2_low_level_write(COMMAND, (descriptor[i] & 0xF0) >> 4);
      sx2_low_level_write(COMMAND, (descriptor[i] & 0x0F));
   }
}

void delay_ms(unsigned short time)
{
   TIMER = _1ms_ * time;
   while(TIMER);
}

void sx2_interrupt_handler(void)
{
   unsigned char i;
   if (INT)
   {
      INT = 0;
      if (read_interrupt)
      {
         read_interrupt = 0;
         return;
      }
      i = sx2_low_level_read(COMMAND);

      switch (i)
      {
         case 0x01: //Ready
            sx2_ready = 1;
            break;

         case 0x02: //Bus Activity
            no_activity = !no_activity;
            break;

         case 0x04: //Enumeration complete
            enum_ok = 1;
            break;

         case 0x20: //Flags
            got_out_data = 1;
            break;

         case 0x40: //EP0Buf
            ep0buf_ready = 1;
            break;

         case 0x80: //Setup
            got_setup = 1;
            break;
      }
   }
}

void sx2_set_register_values(void)
{
   unsigned char i;

   for (i = 0; i < sizeof(regs); i++)
      sx2_write_reg(regs[i], regvalues[i]);
}

void sx2_init(void)
{
   read_interrupt = 0;
   sx2_ready = 0;
   no_activity = 0;
   enum_ok = 0;
   got_out_data = 0;
   ep0buf_ready = 0;
   got_setup = 0;

   // Set I/O Pins
   PPI_DIR = dirIN;
   PPI = 0x00;
   SLRD_H;
   SLWR_H;
   SLOE_H;
   CS_H;
   RESET_H;
   PKTEND_H;
   sx2_set_fifoadr(COMMAND);
}

void sx2_reset(void)
{
   // Reset SX2
   #ifdef DEBUG
      printf("Reset SX2\n");
   #endif
   RESET_L;
   delay_ms(1);
   RESET_H;
   delay_ms(1);
   do{ sx2_interrupt_handler();
   } while(!sx2_ready);
   #ifdef DEBUG
      printf("SX2 is ready\n");
      printf("Setup SX2 with all default values\n");
   #endif
   sx2_set_register_values();

   //////////////////////////////////////////////////////////
   /// Force SX2 to Full-Speed Mode (Write 0x02 into Address 0xE6FB)
   #ifdef DEBUG
      printf("Force Full Speed Mode\n");
   #endif
//   sx2_write_reg(0x3A, 0xFB);
//   sx2_write_reg(0x3B, 0xE6);
//   sx2_write_reg(0x3C, 0x02);
   //////////////////////////////////////////////////////////


   #ifdef DEBUG
      printf("Write descriptor\n");
   #endif
   sx2_write_descriptor();

   do{ sx2_interrupt_handler();
   } while(!enum_ok);

   #ifdef DEBUG
      printf("SX2 has enumerated\n");
   #endif

   sx2_write_reg(0x20, 0xF0); // Flush the FIFO to start
}
