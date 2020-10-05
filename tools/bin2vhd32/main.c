#include <stdio.h>

FILE *fin, *fout;

unsigned char buffer[32768];


unsigned char bytes;
unsigned long mem, addr, fsize;

int main(int argc,char *argv[])
{
//   if(argc <= 1)
//   {
//      printf("Usage: bin2mif yourfile\n");
//      return -1;
//   }

//   fin = fopen(argv[1], "rb");
   fin = fopen("firmware.bin", "rb");
   if(fin == NULL)
   {
      printf("Can't open file %s\n",argv[1]);
      return(0);
   }

   fseek (fin , 0 , SEEK_END);
   fsize = ftell(fin) >> 2;
   rewind(fin);

   fread(buffer, 1, 32768, fin);

   fout = fopen("fastram.vhd", "w");

   fprintf(fout, "library IEEE;\n");
   fprintf(fout, "use IEEE.std_logic_1164.all;\n");
   fprintf(fout, "use IEEE.numeric_std.all;\n\n");
   fprintf(fout, "entity fastram is\n");
   fprintf(fout, "port (\n");
   fprintf(fout, "   CLK      : in  std_logic;\n");
   fprintf(fout, "   A        : in  std_logic_vector(12 downto 0);\n");
   fprintf(fout, "   DI       : in  std_logic_vector(31 downto 0);\n");
   fprintf(fout, "   DO       : out std_logic_vector(31 downto 0);\n");
   fprintf(fout, "   WE       : in  std_logic;\n");
   fprintf(fout, "   MASK     : in  std_logic_vector(3 downto 0) );\n");
   fprintf(fout, "end fastram;\n\n");
   fprintf(fout, "architecture fastram of fastram is\n\n");
   fprintf(fout, "   type RAM_ARRAY is array(0 to 8191) of std_logic_vector(7 downto 0);\n\n");
   fprintf(fout, "   signal RAM3 : RAM_ARRAY := (\n      ");

   int cnt;

   for(cnt=0;cnt<8192;cnt++)
   {
      fprintf(fout, "X\"%02X\"", buffer[cnt*4]);
      if (!(cnt == 8191))
         fprintf(fout, ", ");
      if ((cnt & 0xF) == 0xF)
         fprintf(fout, "\n      ");
   }
   fprintf(fout, " );\n\n   signal RAM2 : RAM_ARRAY := (\n      ");

   for(cnt=0;cnt<8192;cnt++)
   {
      fprintf(fout, "X\"%02X\"", buffer[cnt*4+1]);
      if (!(cnt == 8191))
         fprintf(fout, ", ");
      if ((cnt & 0xF) == 0xF)
         fprintf(fout, "\n      ");
   }
   fprintf(fout, " );\n\n   signal RAM1 : RAM_ARRAY := (\n      ");

   for(cnt=0;cnt<8192;cnt++)
   {
      fprintf(fout, "X\"%02X\"", buffer[cnt*4+2]);
      if (!(cnt == 8191))
         fprintf(fout, ", ");
      if ((cnt & 0xF) == 0xF)
         fprintf(fout, "\n      ");
   }
   fprintf(fout, " );\n\n   signal RAM0 : RAM_ARRAY := (\n      ");

   for(cnt=0;cnt<8192;cnt++)
   {
      fprintf(fout, "X\"%02X\"", buffer[cnt*4+3]);
      if (!(cnt == 8191))
         fprintf(fout, ", ");
      if ((cnt & 0xF) == 0xF)
         fprintf(fout, "\n      ");
   }
   fprintf(fout, " );\n\n");
   fprintf(fout, "begin\n\n");
   fprintf(fout, "   process(CLK)\n");
   fprintf(fout, "   begin\n");
   fprintf(fout, "      if rising_edge(CLK) then\n\n");
   fprintf(fout, "          DO <=   RAM3(to_integer(unsigned(A)))\n");
   fprintf(fout, "                & RAM2(to_integer(unsigned(A)))\n");
   fprintf(fout, "                & RAM1(to_integer(unsigned(A)))\n");
   fprintf(fout, "                & RAM0(to_integer(unsigned(A)));\n\n");
   fprintf(fout, "         if WE = '1' then\n");
   fprintf(fout, "            if MASK(3) = '1' then\n");
   fprintf(fout, "               RAM3(to_integer(unsigned(A))) <= DI(31 downto 24);\n");
   fprintf(fout, "            end if;\n");
   fprintf(fout, "            if MASK(2) = '1' then\n");
   fprintf(fout, "               RAM2(to_integer(unsigned(A))) <= DI(23 downto 16);\n");
   fprintf(fout, "            end if;\n");
   fprintf(fout, "            if MASK(1) = '1' then\n");
   fprintf(fout, "               RAM1(to_integer(unsigned(A))) <= DI(15 downto 8);\n");
   fprintf(fout, "            end if;\n");
   fprintf(fout, "            if MASK(0) = '1' then\n");
   fprintf(fout, "               RAM0(to_integer(unsigned(A))) <= DI(7  downto 0);\n");
   fprintf(fout, "            end if;\n");
   fprintf(fout, "         end if;\n");
   fprintf(fout, "      end if;\n");
   fprintf(fout, "   end process;\n\n");
   fprintf(fout, "end fastram;\n");

   fclose(fout);
   fclose(fin);
   return 0;
}
