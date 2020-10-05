library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL; 
use IEEE.NUMERIC_STD.ALL;   

entity firmware is
port(
   clk            : in    std_logic;
   
   -- LEDs
   led1_red       : out   std_logic;
   led1_green     : out   std_logic;
   led1_blue      : out   std_logic;
   
   led2_red       : out   std_logic;
   led2_green     : out   std_logic;
   led2_blue      : out   std_logic;

   -- USB Device Controller
   usb_d          : inout std_logic_vector(15 downto 0);

   usb_reset      : out   std_logic;
   usb_slwr       : out   std_logic;
   usb_slrd       : out   std_logic;
   usb_sloe       : out   std_logic;
   usb_fifoa0     : out   std_logic;
   usb_fifoa1     : out   std_logic;
   usb_fifoa2     : out   std_logic;
   usb_pktend     : out   std_logic;
   usb_cs         : out   std_logic;

   usb_ready      : in    std_logic;
   usb_int        : in    std_logic;
   usb_flaga      : in    std_logic;
   usb_flagb      : in    std_logic;
   usb_flagc      : in    std_logic;
   usb_vbus       : in    std_logic;
   usb_ifclk      : in    std_logic;
      
   -- User FPGA PS Configuration
   fpga_done      : in    std_logic;
   fpga_init      : in    std_logic;
   fpga_prog      : out   std_logic;

   fpga_din       : out   std_logic;
   fpga_cclk      : out   std_logic;
   
   -- User FPGA SPI Connection
   spi_sck        : out   std_logic;
   spi_mosi       : out   std_logic;
   spi_miso       : in    std_logic;
   spi_csa        : out   std_logic;
   spi_csd        : out   std_logic;
   
   -- User FPGA JTAG Pins
   jtag_ext_tdi   : in    std_logic;
   jtag_ext_tdo   : out   std_logic;
   jtag_ext_tck   : in    std_logic;
   jtag_ext_tms   : in    std_logic;

   jtag_int_tdi   : out   std_logic;
   jtag_int_tdo   : in    std_logic;
   jtag_int_tck   : out   std_logic;
   jtag_int_tms   : out   std_logic );
end;

architecture rtl of firmware is

   signal clk25         : std_logic;
   signal reset         : std_logic := '1';
   signal tick          : unsigned(1 downto 0);

   -- CPU signals
   signal cpu_a_next    : std_logic_vector(31 downto 2);
   signal cpu_we_next   : std_logic_vector(3 downto 0);
   signal cpu_a         : std_logic_vector(31 downto 2);
   signal cpu_we        : std_logic_vector(3 downto 0);
   signal cpu_do        : std_logic_vector(31 downto 0);
   signal cpu_di        : std_logic_vector(31 downto 0);

   -- memory signals
   signal mem_a         : std_logic_vector(12 downto 0);
   signal mem_byte      : std_logic_vector(3 downto 0);
   signal mem_di        : std_logic_vector(31 downto 0);
   signal mem_wr        : std_logic;
   signal mem_do        : std_logic_vector(31 downto 0);

   signal port_do       : std_logic_vector(31 downto 0);   

   signal inputs        : std_logic_vector(31 downto 0);
   signal outputs       : std_logic_vector(31 downto 0);

begin
   
u_fastram : entity work.fastram
port map(
   CLK            => clk,
   A		         => mem_a,
   MASK           => mem_byte,
   DI             => mem_di,
   WE             => mem_wr,
   DO             => mem_do);

u_mips : entity work.mlite_cpu
port map(
   clk            => clk,
   reset_in       => reset,
   intr_in        => '0',
   address_next   => cpu_a_next,
   byte_we_next   => cpu_we_next,
   address        => cpu_a,
   byte_we        => cpu_we,
   data_w         => cpu_do,
   data_r         => cpu_di,
   mem_pause      => '0');

u_o_port : entity work.o_port
generic map(ADDR  => X"80000000",
            DEF   => X"FFFFFFFF")
port map(
   CLK            => clk,
   RESET          => reset,
   BYTE           => cpu_we_next,
   A              => cpu_a_next & "00",
   DI             => cpu_do,
   DO             => port_do,
   OUTPUT         => outputs );

u_i_port : entity work.i_port
generic map(ADDR  => X"80000010")
port map(
   CLK            => clk,
   RESET          => reset,
   BYTE           => cpu_we_next,
   A              => cpu_a_next & "00",
   DI             => cpu_do,
   DO             => port_do,
   INPUT          => inputs );

u_io_port : entity work.io_port
generic map(ADDR  => X"80000020")
port map(
   CLK            => clk,
   RESET          => reset,
   BYTE           => cpu_we_next,
   A              => cpu_a_next & "00",
   DI             => cpu_do,
   DO             => port_do,
   IO             => usb_d );

u_timer : entity work.timer
generic map(ADDR  => X"80000030")
port map(
   CLK            => clk,
   RESET          => reset,
   BYTE           => cpu_we_next,
   A              => cpu_a_next & "00",
   DI             => cpu_do,
   DO             => port_do );

u_loader : entity work.bitfile_loader
generic map(ADDR  => X"80000040")
port map(
   CLK            => clk,
   RESET          => reset,
   BYTE           => cpu_we_next,
   A              => cpu_a_next & "00",
   DI             => cpu_do,
   DO             => port_do,
   CCLK           => fpga_cclk,
   DIN            => fpga_din );

u_interrupt : entity work.interrupt
generic map(ADDR  => X"80000050")
port map(
   CLK            => clk,
   RESET          => reset,
   BYTE           => cpu_we_next,
   A              => cpu_a_next & "00",
   DI             => cpu_do,
   DO             => port_do,
   INT            => usb_int );

u_spi_master : entity work.spi_master
generic map(ADDR  => X"80000060")
port map(
   CLK            => clk,
   RESET          => reset,
   BYTE           => cpu_we_next,
   A              => cpu_a_next & "00",
   DI             => cpu_do,
   DO             => port_do,
   SCK            => spi_sck,
   SDI            => spi_miso,
   SDO            => spi_mosi );
   
-- User FPGA JTAG Pins Forwarding
jtag_int_tdi <= jtag_ext_tdi;
jtag_int_tms <= jtag_ext_tms;
jtag_int_tck <= jtag_ext_tck;
jtag_ext_tdo <= jtag_int_tdo;

-- Status LEDs / Generate Reset Signal
process(clk)
begin
   if rising_edge(clk) then
      led1_red <= '0';
      led1_green <= '0';
      led1_blue <= '0';
      led2_red <= '0';
      led2_green <= '0';
      led2_blue <= '0';
      tick <= tick + 1;
      if tick = "00" then
         if jtag_ext_tck = '1' then
            led1_blue <= '1';
         else
            led1_red <= '1';
         end if;
         if outputs(0) = '1' then
            led2_red <= '1';
         end if;
         if outputs(1) = '1' then
            led2_green <= '1';
         end if;
         if outputs(2) = '1' then
            led2_blue <= '1';
         end if;
      end if;
      if tick = "11" then
         reset <= '0';
      end if;
   end if;
end process;

mem_a <= cpu_a_next(14 downto 2);
mem_byte <= cpu_we_next;     
mem_di <= cpu_do;
cpu_di <= mem_do when cpu_a(31) = '0' else port_do;
mem_wr <= '1' when cpu_we_next /= "0000" and cpu_a_next(31) = '0' else '0';

-- Define outputs
usb_reset   <= outputs(3);
usb_slwr    <= outputs(4);
usb_slrd    <= outputs(5);
usb_sloe    <= outputs(6);
usb_pktend  <= outputs(7);
usb_fifoa0  <= outputs(8);
usb_fifoa1  <= outputs(9);
usb_fifoa2  <= outputs(10);
usb_cs      <= outputs(11);
fpga_prog   <= outputs(12);
spi_csa     <= outputs(13);
spi_csd     <= outputs(14);

-- Define inputs
inputs <= "00000000000000000000000" & fpga_init & fpga_done & usb_ifclk & usb_vbus & usb_flagc & usb_flagb & usb_flaga & usb_int & usb_ready;

end;
