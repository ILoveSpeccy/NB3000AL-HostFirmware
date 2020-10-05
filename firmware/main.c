#include <stdio.h>
#include "sx2.h"
#include "sx2_registers.h"
#include "hardware.h"

#define CMD_RESET_CPU   0x00  // 1 - RESET CPU, 0 - RUN
#define CMD_H_ADDR      0x01  // HIGH SRAM ADDRESS BYTE
#define CMD_M_ADDR      0x02  // MIDDLE SRAM ADDRESS BYTE
#define CMD_L_ADDR      0x03  // LOW SRAM ADDRESS BYTE
#define CMD_3_DATA      0x04  // 31-24 DATA BITS
#define CMD_2_DATA      0x05  // 23-16 DATA BITS
#define CMD_1_DATA      0x06  // 15-8 DATA BITS
#define CMD_0_DATA      0x07  // 7-0 DATA BITS

void spi_data_transfer(unsigned char data)
{
   clear_bit(OUTPUTS, spi_csd);
   SPI_DATA = data;
   while(SPI_STATUS);
   set_bit(OUTPUTS, spi_csd);
}

void spi_addr_transfer(unsigned char addr)
{
   clear_bit(OUTPUTS, spi_csa);
   SPI_DATA = addr;
   while(SPI_STATUS);
   set_bit(OUTPUTS, spi_csa);
}

int main(void)
{
   unsigned char i;
   unsigned long tick = 0;

   set_bit(OUTPUTS, pinLED_R);
   clear_bit(OUTPUTS, pinLED_G);
   clear_bit(OUTPUTS, pinLED_B);

   sx2_init();
   sx2_reset();

   sx2_set_fifoadr(FIFO2);

   // SPI Init
   SPI_CONFIG = 1; // 10MHz SPI Clock
   set_bit(OUTPUTS, spi_csa);
   set_bit(OUTPUTS, spi_csd);

   while(1)
   {
      while (bit_is_clear(INPUTS, pinFLAGA))
      {
         tick++;
         if (tick & 0x20000)
            clear_bit(OUTPUTS, pinLED_R);
         else
            set_bit(OUTPUTS, pinLED_R);
      }
      switch(sx2_fifo_read())
      {
         case 0xF0:  // Init FPGA
            for(i=0;i<63;i++)
               sx2_fifo_read();

            clear_bit(OUTPUTS, fpga_prog);
            delay_ms(1);
            set_bit(OUTPUTS, fpga_prog);
            while(bit_is_clear(INPUTS, fpga_init));
            break;

         case 0xF1:
            set_bit(OUTPUTS, pinLED_G);
            for(i=0;i<63;i++)
               CONF_DATA = sx2_fifo_read();
               while(CONF_STATUS);
            clear_bit(OUTPUTS, pinLED_G);
            break;

         case 0xF2:  // Write Adress
            set_bit(OUTPUTS, pinLED_B);
            spi_addr_transfer(sx2_fifo_read());
            for(i=0;i<62;i++)
               sx2_fifo_read();
            clear_bit(OUTPUTS, pinLED_B);
            break;

         case 0xF3:  // Write Data
            set_bit(OUTPUTS, pinLED_B);
            spi_data_transfer(sx2_fifo_read());
            for(i=0;i<62;i++)
               sx2_fifo_read();
            clear_bit(OUTPUTS, pinLED_B);
            break;

         case 0xF4:  // Write 4 Bytes Data
            set_bit(OUTPUTS, pinLED_B);
            spi_addr_transfer(CMD_3_DATA);
            spi_data_transfer(sx2_fifo_read());
            spi_addr_transfer(CMD_2_DATA);
            spi_data_transfer(sx2_fifo_read());
            spi_addr_transfer(CMD_1_DATA);
            spi_data_transfer(sx2_fifo_read());
            spi_addr_transfer(CMD_0_DATA);
            spi_data_transfer(sx2_fifo_read());
            for(i=0;i<59;i++)
               sx2_fifo_read();
            clear_bit(OUTPUTS, pinLED_B);
            break;

         case 0xF5:  // Write 15x 32-bit Words Data
            set_bit(OUTPUTS, pinLED_B);
            for(i=0;i<15;i++)
            {
               spi_addr_transfer(CMD_3_DATA);
               spi_data_transfer(sx2_fifo_read());
               spi_addr_transfer(CMD_2_DATA);
               spi_data_transfer(sx2_fifo_read());
               spi_addr_transfer(CMD_1_DATA);
               spi_data_transfer(sx2_fifo_read());
               spi_addr_transfer(CMD_0_DATA);
               spi_data_transfer(sx2_fifo_read());
            }
            for(i=0;i<3;i++)
               sx2_fifo_read();
            clear_bit(OUTPUTS, pinLED_B);
            break;

         default:
            break;
      }
   }

   return 0;
}
