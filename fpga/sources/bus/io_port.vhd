library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL; 

entity io_port is
   generic (ADDR : std_logic_vector := X"00000000");
   port(
      CLK      : in  std_logic;
      RESET    : in  std_logic;
      BYTE     : in  std_logic_vector(3 downto 0);
      A        : in  std_logic_vector(31 downto 0);
      DI       : in  std_logic_vector(31 downto 0);
      DO       : out std_logic_vector(31 downto 0);
      IO       : inout std_logic_vector(15 downto 0) );
end;

architecture logic of io_port is

   signal O_REG : std_logic_vector(15 downto 0);
   signal DIR   : std_logic;

begin

   process(CLK)
   begin
      if rising_edge(CLK) then
         DO <= (OTHERS=>'Z');
         if RESET = '1' then
            DIR <= '0';
         else
            if A = ADDR then
               if BYTE = "0000" then
                  if DIR = '0' then
                     DO <= "0000000000000000" & IO;
                  else
                     DO <= "0000000000000000" & O_REG;                  
                  end if;
               else
                  O_REG <= DI(15 downto 0);
               end if;
            end if;
            if A = std_logic_vector(unsigned(ADDR) + 4) and BYTE /= "0000" then
               DIR <= DI(0);
            end if;
         end if;
      end if;
   end process;

   IO <= O_REG when DIR = '1' else (OTHERS=>'Z');
   
end;
