//Trabalho 1 - Multimídia e Hipermídia
//Leonardo Prado Dias - N°USP: 10684642
//Renan Peres Martins - N°USP: 10716612
#include <stdio.h>
#include <stdlib.h>
#include "codificacao_GEPJ.h"

int main(int argc, char *argv[])
{
  FILE *input, *output;
  BITMAPFILEHEADER FileHeader;       /* File header */
  BITMAPINFOHEADER InfoHeader;       /* Info header */
  PIXEL *Image;
  TABELA *TabCodigos;
  int i;
         
  if(!(input = fopen(argv[1], "rb"))){
          printf("Error: could not open input file.\n" );
          exit(1);
  }
  
  loadBMPHeaders (input, &FileHeader, &InfoHeader);
  
  Image = (PIXEL *) calloc((InfoHeader.Width * InfoHeader.Height), sizeof(PIXEL));
  TabCodigos = (TABELA *) calloc((InfoHeader.Width * InfoHeader.Height * 3), sizeof(TABELA));

  loadBMPImage(input, InfoHeader, Image);
                               

  if(!(output = fopen("out.bmp", "wb"))){
          printf("Error: could not open ""out.bin"" file." );
          exit(1);
  }

  fseek(input, 0, SEEK_SET);
  for(i=0; i<54; i++) fputc(fgetc(input), output);

  printf("\nDefina o método de descompressao:\n\n");
  printf("Sem perdas (Compressao por diferença + Huffman) -> 1\n");
  printf("Com perdas (DCT + Quantização) -> 2\n");
  printf("Encerrar descompressor -> 0\n\n");
  i = 1;
  while(i != 0){
    printf("Sua escolha: ");
    scanf("%d", &i);
    if( i == 1){
      TabCodigos = CodDiferencial(Image,InfoHeader.Height,InfoHeader.Width);
      Image = DecodDiferencial(TabCodigos,InfoHeader.Height,InfoHeader.Width);
      i = 0;
    }else if(i == 2){
      TabCodigos = Cod_GEPJ(Image,InfoHeader);
      Image = Decod_GEPJ(TabCodigos, InfoHeader);
      i = 0;
    }else if(i != 0) printf("Opção inválida! Tente novamente!");
  }
  for (i=0; i < (InfoHeader.Height * InfoHeader.Width); i++){
    fputc(Image[i].B, output);
    fputc(Image[i].G, output);
    fputc(Image[i].R, output); 
  }
  printf("Novo Bitmap gerado:\n");
  printHeaders(&FileHeader, &InfoHeader);

  fclose(input);
  fclose(output);
  return 0;
}