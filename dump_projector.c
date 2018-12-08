#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   int szFile;
   char dataEnd[8];
   uint32_t lpNumberOfBytesRW;
   void *hfInput,
        *hfOutput,
        *hMemHeap;

   if (argc != 3) {
      printf("Usage: dump_projector infile outfile\r\n\
Example: `dump_projector application.exe application.swf`\r\n");
      return 1;
   }

   hfInput = fopen(argv[1], "rb");
   if (hfInput == NULL) {
      printf("Cannot open input file for reading!\r\n");
   } else {
      fseek(hfInput, -8L, SEEK_END);
      szFile = ftell(hfInput);
      fread(dataEnd, 8, 1, hfInput);
      if (*(uint32_t*)&dataEnd == 0xFA123456) { // Check the signature
	 hMemHeap = calloc(1, *(uint32_t*)&dataEnd[4]);
         if (hMemHeap != NULL) {
	    fseek(hfInput, szFile-*(uint32_t*)&dataEnd[4], SEEK_SET);
            lpNumberOfBytesRW = fread(hMemHeap, 1, *(uint32_t*)&dataEnd[4], hfInput);
            hfOutput = fopen(argv[2], "wb");
            if (hfOutput == NULL) {
               printf("Cannot create output file for writing!\r\n");
            } else {
               fwrite(hMemHeap, 1, *(uint32_t*)&dataEnd[4], hfOutput);
               fclose(hfOutput);
               printf("Done, check your shiny new %s! =)\r\n", argv[2]);
            }
            free(hMemHeap);
         } else {
            printf("Can't fetch the process heap!\r\n");
         }
      } else {
         printf("This doesn't seems to be a projector bundle.\r\n");
      }
      fclose(hfInput);
   }
   return 0;
}
