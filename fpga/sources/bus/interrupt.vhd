library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL; 

entity interrupt is
   generic (ADDR : std_logic_vector := X"00000000");
   port(
      CLK      : in  std_logic;
      RESET    : in  std_logic;
      BYTE     : in  std_logic_vector(3 downto 0);
      A        : in  std_logic_vector(31 downto 0);
      DI       : in  std_logic_vector(31 downto 0);
      DO       : out std_logic_vector(31 downto 0);
      INT      : in    std_logic );
end;

architecture logic of interrupt is

   signal INT_BUF  : std_logic_vector(3 downto 0);
   signal STATUS   : std_logic;
   
begin

   process(CLK)
   begin
      if rising_edge(CLK) then
         DO <= (OTHERS=>'Z');
         if RESET = '1' then
            INT_BUF <= "1111";
            STATUS <= '0';
         else
            INT_BUF <= INT_BUF(2 downto 0) & INT;
           
            if INT_BUF = "1100" then
               STATUS <= '1';
            end if;
            
            if A = ADDR then
               if BYTE = "0000" then
                  DO <= "0000000000000000000000000000000" & STATUS;
               else
                  STATUS <= '0';
               end if;
            end if;
         end if;
      end if;
   end process;
   
end;
