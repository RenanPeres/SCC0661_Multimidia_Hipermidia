#include <stdio.h>
#include <stdlib.h>
#include "codificacao_GEPJ.h"

void compara();
void charToBit(char ch);

int main(int argc, char *argv[])
{
  FILE *input, *output, *teste;
  BITMAPFILEHEADER FileHeader;
  BITMAPINFOHEADER InfoHeader;
  PIXEL *Image;
  TABELA *TabCodigos;
  int i;

  if(!(teste = fopen("teste.txt", "w"))){
          printf("Error: could not open input file.\n" );
          exit(1);
  }
         
  if(!(input = fopen(argv[1], "rb"))){
          printf("Error: could not open input file.\n" );
          exit(1);
  }
  
  loadBMPHeaders (input, &FileHeader, &InfoHeader);
  printf("Bitmap lido:\n");
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
  }GravaBit(TabCodigos, (InfoHeader.Height * InfoHeader.Width), output, teste);
  
  fclose(teste);
  fclose(input);
  fclose(output);
  //compara();
  return 0;
}

void compara()
{
    FILE *output;
    BITMAPFILEHEADER FileHeader;       /* File header */
    BITMAPINFOHEADER InfoHeader;       /* Info header */
    char c;
    if(!(output = fopen("out.bin", "rb"))){
          printf("Error: could not open ""out.bin"" file." );
          exit(1);
  }loadBMPHeaders (output, &FileHeader, &InfoHeader);
  for(int i = 0; i < 18; i++){
    fread(&c, sizeof(char), 1, output);
    charToBit(c);
  }
}

void charToBit(char ch)
{
    for (int i = 7; i >= 0; --i)
    {
        putchar( (ch & (1 << i)) ? '1' : '0' );
    }
    putchar('\n');
}