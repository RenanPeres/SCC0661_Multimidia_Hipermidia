//Trabalho 1 - Multimídia e Hipermídia
//Leonardo Prado Dias - N°USP: 10684642
//Renan Peres Martins - N°USP: 10716612
#include <stdio.h>
#include <stdlib.h>
#include "codificacao_GEPJ.h"

int main(int argc, char *argv[])
{
  FILE *input, *output;
  BITMAPFILEHEADER FileHeader;
  BITMAPINFOHEADER InfoHeader;
  PIXEL *Image;
  TABELA *TabCodigos;
  int i;
  
  if(!(input = fopen(argv[1], "rb"))){
          printf("Error: could not open input file.\n" );
          exit(1);
  }
  
  loadBMPHeaders (input, &FileHeader, &InfoHeader);
  printHeaders(&FileHeader, &InfoHeader);
  
  Image = (PIXEL *) malloc((InfoHeader.Width * InfoHeader.Height) * sizeof(PIXEL));

  loadBMPImage(input, InfoHeader, Image);                          

  if(!(output = fopen("out.bin", "wb"))){
          printf("Error: could not open ""out.bin"" file." );
          exit(1);
  }

  InfoHeader.Compression = 1;

  fseek(input, 0, SEEK_SET);
  for(i=0; i<54; i++) putc(fgetc(input), output);

  printf("\nDefina o método de compressao:\n\n");
  printf("Sem perdas (Compressao por diferença + Huffman) -> 1\n");
  printf("Com perdas (DCT + Quantização) -> 2\n");
  printf("Encerrar compressor -> 0\n\n");
  i = 1;
  while(i != 0){
    printf("Sua escolha: ");
    scanf("%d", &i);
    if( i == 1){
      TabCodigos = CodDiferencial(Image, InfoHeader.Height, InfoHeader.Width);
      i = 0;
    }else if(i == 2){
      TabCodigos = Cod_GEPJ(Image, InfoHeader);
      i = 0;
    }else if(i != 0) printf("Opção inválida! Tente novamente!");
  }GravaBit(TabCodigos, (InfoHeader.Height * InfoHeader.Width), output);
  
  fclose(input);
  fclose(output);

  return 0;
}