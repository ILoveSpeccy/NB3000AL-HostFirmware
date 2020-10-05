library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL; 

entity o_port is
   generic (ADDR : std_logic_vector := X"00000000";
            DEF  : std_logic_vector := X"00000000" );
   port(
      CLK      : in  std_logic;
      RESET    : in  std_logic;
      BYTE     : in  std_logic_vector(3 downto 0);
      A        : in  std_logic_vector(31 downto 0);
      DI       : in  std_logic_vector(31 downto 0);
      DO       : out std_logic_vector(31 downto 0);
      OUTPUT   : out std_logic_vector(31 downto 0) );
end;

architecture logic of o_port is

   signal OUTPUT_REG : std_logic_vector(31 downto 0);

begin
   
   OUTPUT <= OUTPUT_REG;
   
   process(CLK)
   begin
      if rising_edge(CLK) then
         DO <= (OTHERS=>'Z');
         if RESET = '1' then
            OUTPUT_REG <= DEF;
         else
            if A = ADDR then
               if BYTE /= "0000" then
                  OUTPUT_REG <= DI;
               else
                  DO <= OUTPUT_REG;
               end if;
            end if;
         end if;
      end if;
   end process;

end;
