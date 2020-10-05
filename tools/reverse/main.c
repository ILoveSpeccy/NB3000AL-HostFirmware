#include <stdio.h>
#include <string.h>

FILE *fin;
FILE *fout;

unsigned char value;
signed long count = -1;

unsigned char reverse(unsigned char b)
{
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

int main(int argc,char *argv[])
{
   if (argc < 3)
   {
      printf("Usage: reverse <input file> <output file>\n");
      return -1;
   }

   fin = fopen(argv[1], "rb");

   if (fin == NULL)
   {
      printf("Can't open file %s\n", argv[1]);
      return -1;
   }

   fout = fopen(argv[2], "wb");

   while(fread(&value, 1, 1, fin))
   {
       value = reverse(value);
       fwrite(&value, 1, 1, fout);
   }

   fclose(fin);
   fclose(fout);

   return 0;
}


