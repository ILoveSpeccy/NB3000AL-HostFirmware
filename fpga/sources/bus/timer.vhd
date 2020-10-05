library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL; 
use IEEE.NUMERIC_STD.ALL; 

entity timer is
   generic (ADDR  : std_logic_vector := X"00000000");
   port(
      CLK      : in  std_logic;
      RESET    : in  std_logic;
      BYTE     : in  std_logic_vector(3 downto 0);
      A        : in  std_logic_vector(31 downto 0);
      DI       : in  std_logic_vector(31 downto 0);
      DO       : out std_logic_vector(31 downto 0) );
end timer;

architecture behavioral of timer is

   signal counter : unsigned(31 downto 0);  
   signal load : std_logic;

begin                 

   -- Control
   process(CLK)
   begin
      if rising_edge(CLK) then 
         load <= '0';
         DO <= (OTHERS=>'Z');
         if RESET = '1' then
            counter <= X"00000000";
         else
            if A = ADDR then
               if BYTE = "0000" then
                  DO <= std_logic_vector(counter);
               else
                  load <= '1';
               end if;
            end if;
         end if;
         
         if load = '1' then
            counter <= unsigned(DI);
         else
            if counter /= X"00000000" then
               counter <= counter - 1;
            end if;
         end if;
      
      end if;
   end process;
   
end behavioral;
