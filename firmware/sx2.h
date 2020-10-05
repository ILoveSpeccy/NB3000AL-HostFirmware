#ifndef __SX2_H__
#define	__SX2_H__

   volatile unsigned char read_interrupt;
   volatile unsigned char sx2_ready;
   volatile unsigned char no_activity;
   volatile unsigned char enum_ok;
   volatile unsigned char got_out_data;
   volatile unsigned char ep0buf_ready;
   volatile unsigned char got_setup;

///   #define DEBUG

   // Low level read/write type
   #define FIFO2     0x00
   #define FIFO4     0x01
   #define FIFO6     0x02
   #define FIFO8     0x03
   #define COMMAND   0x04

   // Output Pins Configuration
   #define SLRD_H    set_bit(OUTPUTS, pinSLRD)
   #define SLWR_H    set_bit(OUTPUTS, pinSLWR)
   #define SLOE_H    set_bit(OUTPUTS, pinSLOE)
   #define CS_H      set_bit(OUTPUTS, pinCS)
   #define RESET_H   set_bit(OUTPUTS, pinRESET)
   #define PKTEND_H  set_bit(OUTPUTS, pinPKTEND)
   #define A0_H      set_bit(OUTPUTS, pinFIFOA0)
   #define A1_H      set_bit(OUTPUTS, pinFIFOA1)
   #define A2_H      set_bit(OUTPUTS, pinFIFOA2)

   #define SLRD_L    clear_bit(OUTPUTS, pinSLRD)
   #define SLWR_L    clear_bit(OUTPUTS, pinSLWR)
   #define SLOE_L    clear_bit(OUTPUTS, pinSLOE)
   #define CS_L      clear_bit(OUTPUTS, pinCS)
   #define RESET_L   clear_bit(OUTPUTS, pinRESET)
   #define PKTEND_L  clear_bit(OUTPUTS, pinPKTEND)
   #define A0_L      clear_bit(OUTPUTS, pinFIFOA0)
   #define A1_L      clear_bit(OUTPUTS, pinFIFOA1)
   #define A2_L      clear_bit(OUTPUTS, pinFIFOA2)

   // Input Pins Configuration
   #define statINT   (bit_is_set(INPUTS, pinINT))
   #define statREADY (bit_is_set(INPUTS, pinREADY))

   // Interrupt types
   #define intReady  0x01
   #define intBusAct 0x02
   #define intEnumOk 0x04
   #define intFlags  0x20
   #define intEP0BUF 0x40
   #define intSetup  0x80

   // Read/Write Command Request Definitions
   #define ADDRESS   0x0080
   #define READ      0x0040
   #define WR_REQ    ADDRESS
   #define RD_REQ    (ADDRESS | READ)

   void sx2_set_fifoadr(unsigned char addr);
   void sx2_low_level_write(unsigned char addr, unsigned char data);
   unsigned char sx2_low_level_read(unsigned char addr);
   unsigned char sx2_fifo_read(void);
   void sx2_write_descriptor(void);
   void sx2_init(void);
   void delay_ms(unsigned short time);
   void sx2_interrupt_handler(void);
   void sx2_set_register_values(void);
   unsigned char sx2_read_reg(unsigned char reg);
   void sx2_write_reg(unsigned char reg, unsigned char data);
   void sx2_reset(void);

#endif
