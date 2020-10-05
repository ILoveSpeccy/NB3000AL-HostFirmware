library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL; 

entity i_port is
   generic (ADDR : std_logic_vector := X"00000000");
   port(
      CLK      : in  std_logic;
      RESET    : in  std_logic;
      BYTE     : in  std_logic_vector(3 downto 0);
      A        : in  std_logic_vector(31 downto 0);
      DI       : in  std_logic_vector(31 downto 0);
      DO       : out std_logic_vector(31 downto 0);
      INPUT    : in  std_logic_vector(31 downto 0) );
end;

architecture logic of i_port is

   signal INPUT_REG : std_logic_vector(31 downto 0);

begin

   INPUT_REG <= INPUT;

   process(CLK)
   begin
      if rising_edge(CLK) then
         DO <= (OTHERS=>'Z');
         if A = ADDR then
            if BYTE = "0000" then
               DO <= INPUT_REG;
            end if;
         end if;
      end if;
   end process;

end;
