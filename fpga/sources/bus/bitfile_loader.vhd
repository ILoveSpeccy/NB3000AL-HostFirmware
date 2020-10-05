library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;

entity bitfile_loader is
   generic (ADDR  : std_logic_vector := X"00000000");
   port(
      CLK      : in  std_logic;
      RESET    : in  std_logic;
      BYTE     : in  std_logic_vector(3 downto 0);
      A        : in  std_logic_vector(31 downto 0);
      DI       : in  std_logic_vector(31 downto 0);
      DO       : out std_logic_vector(31 downto 0);
      CCLK     : out std_logic;
      DIN      : out std_logic );
end;

architecture Behavioral of bitfile_loader is

   signal start   : std_logic;
   signal busy    : std_logic;
   signal data_i  : std_logic_vector(7 downto 0);
   signal tick    : unsigned(4 downto 0);

   type spi_states is (IDLE, RUN, DONE);  
   signal spi_state : spi_states := IDLE;

begin

   -- Control
   process(CLK)
   begin
      if rising_edge(CLK) then
         start <= '0';
         DO <= (OTHERS=>'Z');
         if BYTE /= "0000" then
            if A = ADDR then
               data_i <= DI(7 downto 0);
               start <= '1';
            end if;
         else
            if A = ADDR then
               DO <= "0000000000000000000000000000000" & busy;
            end if;
         end if;
      end if;
   end process;

   process(CLK)
   begin       
      
      if rising_edge(CLK) then
         if RESET = '1' then                   
            busy <= '0';      
            CCLK <= '0';
            DIN <= '0';
         else
            case spi_state is
               when IDLE =>
                  if start = '1' then
                     busy <= '1';
                     tick <= "00000";
                     spi_state <= RUN;
                     
                  end if;
               
               when RUN =>               
               
                  tick <= tick + 1;
                  
                  case tick is

                     when "00000" =>
                        DIN <= data_i(0);
                     when "00011" =>
                        DIN <= data_i(1);
                     when "00110" =>
                        DIN <= data_i(2);
                     when "01001" =>
                        DIN <= data_i(3);
                     when "01100" =>
                        DIN <= data_i(4);
                     when "01111" =>
                        DIN <= data_i(5);
                     when "10010" =>
                        DIN <= data_i(6);
                     when "10110" =>
                        DIN <= data_i(7);
                     when "00001" | "00100" | "00111" | "01010" | "01101" | "10000" | "10011" | "10111" =>
                        CCLK <= '1';
                     when "00010" | "00101" | "01000" | "01011" | "01110" | "10001" | "10100" | "11000" =>
                        CCLK <= '0';
                     when others => null;
                  
                  end case;
                  
                  if tick = "11000" then
                     spi_state <= DONE;   
                  end if;
                                             
               when DONE =>
                  busy <= '0';
                  CCLK <= '0';
                  DIN <= '0';
                  spi_state <= IDLE;   

               when OTHERS =>
                  spi_state <= IDLE;

            end case;
         end if;
      end if;
      
   end process;     
   
end Behavioral;
