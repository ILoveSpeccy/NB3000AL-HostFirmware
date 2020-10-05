#define set_bit(var, bit)        ((var) |= (1 << (bit)))
#define clear_bit(var, bit)      ((var) &= (unsigned)~(1 << (bit)))
#define toggle_bit(var,bit)      ((var) ^= (1 << (bit)))

#define bit_is_set(var, bit)     ((var) & (1 << (bit)))
#define bit_is_clear(var, bit)   (!((var) & (1 << (bit))))

#define MEM_READ(A)     (*(volatile unsigned int*)(A))
#define MEM_WRITE(A,V)  (*(volatile unsigned int*)(A))=(V)

// Outputs
#define OUTPUTS         (*(volatile unsigned int*)(0x80000000))

#define pinLED_R        0
#define pinLED_G        1
#define pinLED_B        2
#define pinRESET        3
#define pinSLWR         4
#define pinSLRD         5
#define pinSLOE         6
#define pinPKTEND       7
#define pinFIFOA0       8
#define pinFIFOA1       9
#define pinFIFOA2       10
#define pinCS           11
#define fpga_prog       12
#define spi_csa         13
#define spi_csd         14

// Inputs
#define INPUTS          (*(volatile unsigned int*)(0x80000010))

#define pinREADY        0
#define pinINT          1
#define pinFLAGA        2
#define fpga_done       7
#define fpga_init       8

//PPI_DIR 0 = IN, 1 = OUT
#define PPI             (*(volatile unsigned int*)(0x80000020))
#define PPI_DIR         (*(volatile unsigned int*)(0x80000024))
#define dirIN           0
#define dirOUT          1

// Timer
#define _1ms_           20000
#define TIMER           (*(volatile unsigned int*)(0x80000030))

#define CONF_DATA       (*(volatile unsigned int*)(0x80000040))
#define CONF_STATUS     (*(volatile unsigned int*)(0x80000040))

// Interrupt
#define INT             (*(volatile unsigned int*)(0x80000050))

// SPI
#define SPI_DATA        (*(volatile unsigned int*)(0x80000050))
#define SPI_CONFIG      (*(volatile unsigned int*)(0x80000054))
#define SPI_STATUS      (*(volatile unsigned int*)(0x80000054))
