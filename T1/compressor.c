#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "codificacao_dif.h"
#include "codificacao_GEPJ.h"


int main(int argc, char *argv[])
{
  FILE *input, *output;
  int i;
  unsigned char *v, modo;
  
  FILE *fp;
  BITMAPFILEHEADER FileHeader;       /* File header */
  BITMAPINFOHEADER InfoHeader;       /* Info header */
  PIXEL *Image;
         
  if(!(input = fopen(argv[1], "rb"))){
          printf("Error: could not open input file.\n" );
          exit(1);
  }
  
  loadBMPHeaders (input, &FileHeader, &InfoHeader);
  
  Image = (PIXEL *) malloc((InfoHeader.Width * InfoHeader.Height) * sizeof(PIXEL));
  TABELA *TabCodigos;

  loadBMPImage(input, InfoHeader, Image);
                               

  if(!(output = fopen("out.bin", "wb"))){
          printf("Error: could not open ""out.bin"" file." );
          exit(1);
  }
  
  fseek(input, 0, SEEK_SET);
  for(i=0; i<54; i++)
           fputc(fgetc(input), output);

  

  printf("\nDefina o método de compressao:\n\n");
  printf("Sem perdas (Compressao por diferença + Huffman) -> 1\n");
  printf("Com perdas (DCT + Quantização) -> 2\n");
  printf("Encerrar compressor -> 0\n\n");
  while(1){
    printf("Sua escolha: ");
    modo = getc(stdin);
    if(modo == '1'){ 
    //  decod_dif();
      break;
    }else if(modo == '2'){
      decod_jpeg(Image, InfoHeader, TabCodigos);
    }else if(modo =='0') break;
    else printf("Opção inválida! Tente novamente!");
  }GravaBit(TabCodigos, (InfoHeader.Height * InfoHeader.Width), output, teste);
  

  fclose(input);
  fclose(output);
  return 0;
}