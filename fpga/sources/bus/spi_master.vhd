library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.ALL;

-- ==================================
--       RD             WR
-- ----------------------------------
--   0   Read Data      Write Data
--   4   Status         SPI Speed
-- ==================================

entity SPI_MASTER is
   generic (ADDR : std_logic_vector := X"00000000");
   port(
      CLK      : in  std_logic;
      RESET    : in  std_logic;
      BYTE     : in  std_logic_vector(3 downto 0);
      A        : in  std_logic_vector(31 downto 0);
      DI       : in  std_logic_vector(31 downto 0);
      DO       : out std_logic_vector(31 downto 0);

      SCK      : out std_logic;
      SDI      : in  std_logic;
      SDO      : out std_logic );
end;

architecture Behavioral of SPI_MASTER is

   signal speed   : std_logic_vector(7 downto 0);
   signal start   : std_logic;
   signal busy    : std_logic;
   signal data_i  : std_logic_vector(7 downto 0);
   signal data_o  : std_logic_vector(7 downto 0);

   signal delay : std_logic_vector(7 downto 0);  
   signal tick  : unsigned(3 downto 0);
   signal sck_p : std_logic;
   signal sck_n : std_logic;   
   signal tx_data : std_logic_vector(7 downto 0);
   signal rx_data : std_logic_vector(7 downto 0);

   type spi_states is (IDLE, RUN, DONE);  
   signal spi_state : spi_states := IDLE;

begin

   -- Control
   process(CLK)
   begin
      if rising_edge(CLK) then
         start <= '0';
         DO <= (OTHERS=>'Z');
         if RESET = '1' then
            DO <= (OTHERS=>'0');
            speed <= "11111111";
         else
            if BYTE /= "0000" then
               if A = ADDR then
                  data_i <= DI(7 downto 0);
                  start <= '1';
               elsif A = std_logic_vector(unsigned(ADDR) + 4) then
                  speed <= DI(7 downto 0);
               end if;
            else
               if A = ADDR then
                  DO <= data_o & data_o & data_o & data_o;
               elsif A = std_logic_vector(unsigned(ADDR) + 4) then
                  DO <= "0000000000000000000000000000000" & busy;
               end if;
            end if;
         end if;
      end if;
   end process;

   process(CLK)
   begin       
      
      if rising_edge(CLK) then
      
         if RESET = '1' then                   
            
            busy <= '0';      
            sck_p <= '0';
            sck_n <= '0';
            
         else                                  

            sck_p <= '0';
            sck_n <= '0';

            case spi_state is
               
               when IDLE =>
               
                  if start = '1' then
                     
                     busy <= '1';
                     delay <= "00000000";   
                     tick <= "0000";
                     spi_state <= RUN;
                     
                  end if;
               
               when RUN =>               
               
                  delay <= delay + '1';  
                  
                  if delay = X"00" then 
                     
                     if tick(0) = '1' then 
                        sck_p <= '1';
                     else
                        sck_n <= '1';
                     end if;  
                     
                  end if;
                     
                  if delay = speed then
                  
                     delay <= "00000000";
                     tick <= tick + 1;
                     if tick = "1111" then
                        spi_state <= DONE;   
                     end if;
                                             
                  end if;   
                  
               when DONE =>
                  busy <= '0';
                  spi_state <= IDLE;   
                  data_o <= rx_data;
               
               when OTHERS =>
               
                  spi_state <= IDLE;
               
            end case;
            
         end if;
         
      end if;
      
   end process;     
   
   process(CLK)
   begin       
      if rising_edge(CLK) then
         if spi_state = IDLE then
            tx_data <= data_i;
         elsif sck_p = '1' then
            tx_data <= tx_data(6 downto 0) & '0';
         end if;
      end if;
   end process;  

   process(CLK)
   begin       
      if rising_edge(CLK) then
         if sck_n = '1' then
            rx_data <= rx_data(6 downto 0) & SDI;
         end if;
      end if;
   end process;  

   process(CLK)
   begin       
      if rising_edge(CLK) then 
         if RESET = '1' then
            SCK <= '0';
         else
            if sck_n = '1' then 
               SCK <= '1';
            elsif sck_p = '1' then
               SCK <= '0';
            end if;
         end if;
      end if;
   end process;  
   
   SDO <= tx_data(7);

end Behavioral;
