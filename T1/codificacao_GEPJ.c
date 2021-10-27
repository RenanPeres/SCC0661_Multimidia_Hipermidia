#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "codificacao_dif.h"
#include "codificacao_GEPJ.h"


/* int main(int argc, char *argv[])
{
  FILE *input, *cinza, *output;
  int i;
  unsigned char *v, modo;
  
  FILE *fp;
  BITMAPFILEHEADER FileHeader;  
  BITMAPINFOHEADER InfoHeader;   
  PIXEL *Image;
  
  if(!(input = fopen(argv[1], "rb"))){
          printf("Error: could not open input file.\n" );
          exit(1);
  }
  
  loadBMPHeaders (input, &FileHeader, &InfoHeader);
  
  Image = (PIXEL *) malloc((InfoHeader.Width * InfoHeader.Height) * sizeof(PIXEL));
  
  loadBMPImage(input, InfoHeader, Image);
                           

  if(!(output = fopen("out.bmp", "wb"))){
            printf("Error: could not open output file." );
            exit(1);
  }

  int *dados;
  dados = (int*)calloc(3 * InfoHeader.Height * InfoHeader.Width, sizeof(int));

  fseek(input, 0, SEEK_SET);
  for(i=0; i<54; i++)
           fputc(fgetc(input), output);
  fclose(input);

  cod_GEPJ(Image, InfoHeader, dados);

  decod_GEPJ(Image, InfoHeader, dados);

  for (i=0; i < (InfoHeader.Height * InfoHeader.Width); i++){
       fputc(Image[i].B, output);
       fputc(Image[i].G, output);
       fputc(Image[i].R, output); 
  }

  fclose(output);
  return 0;
} */

void cod_GEPJ(PIXEL *Image, BITMAPINFOHEADER InfoHeader, TABELA *Tabela)
{
  int matrix[64];
  

  //Para os valores de B
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    for(int j = 0; j < 64; j++) matrix[j] = Image[i+j].B;
    dct(matrix);
    quant(matrix);
    for(int j = 0; j < 64; j++) Image[i+j].B = matrix[ZZ_Matrix[j]];
  }//Para os valores de G
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    for(int j = 0; j < 64; j++) matrix[j] = Image[i+j].G;
    dct(matrix);
    quant(matrix);
    for(int j = 0; j < 64; j++) Image[i + j].G = matrix[ZZ_Matrix[j]];
  }//Para os valores de R
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    for(int j = 0; j < 64; j++) matrix[j] = Image[i+j].R;
    dct(matrix);
    quant(matrix);
    for(int j = 0; j < 64; j++) Image[i + j].R = matrix[ZZ_Matrix[j]];
  }Tabela = CodDiferencial(Image, InfoHeader.Height, InfoHeader.Width);

  return;
}

void decod_GEPJ(PIXEL *Image, BITMAPINFOHEADER InfoHeader)
{
  int matrix[64];

  //iDiferencial
  //Para os valores de B
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    for(int j = 0; j < 64; j++) matrix[ZZ_Matrix[j]] = Image[i+j].B;
    iquant(matrix);
    idct(matrix);
    for(int j = 0; j < 64; j++) Image[i+j].B = matrix[j];
  }//Para os valores de G
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    for(int j = 0; j < 64; j++) matrix[ZZ_Matrix[j]] = Image[i + j].G;
    iquant(matrix);
    idct(matrix);
    for(int j = 0; j < 64; j++) Image[i+j].G = matrix[j];
  }//Para os valores de R
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    for(int j = 0; j < 64; j++) matrix[ZZ_Matrix[j]] = Image[i + j].R;
    iquant(matrix);
    idct(matrix);
    for(int j = 0; j < 64; j++) Image[i+j].R = matrix[j];
  }

  return;
}

void dct(int* matrix)
{
  float result[64];

  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      result[i*8+j] = 0;
      for(int k = 0; k < 8; k++) result[i*8+j] += (DCT_Matrix[i][k] * matrix[k*8+j]);
    }
  }
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      matrix[i*8+j] = 0;
      for(int k = 0; k < 8; k++) matrix[i*8+j] += (int)(result[i*8+k] * DCT_Matrix[j][k]);
    }
  }
}

void idct(int* matrix)
{
  float result[64];  

  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      result[i*8+j] = 0;
      for(int k = 0; k < 8; k++) result[i*8+j] += (matrix[i*8+k] * DCT_Matrix[k][j]);
    }
  }
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      matrix[i*8+j] = 0;
      for(int k = 0; k < 8; k++) matrix[i*8+j] += (int)(result[k*8+j] * DCT_Matrix[k][i]);
    }
  }

}

void quant(int* matrix)
{
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      matrix[i*8+j] = (int)matrix[i*8+j]/(QUANT_Matrix[i][j]);
    }
  }
}

void iquant(int* matrix)
{
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      matrix[i*8+j] *= QUANT_Matrix[i][j];
    }
  }
}

void cod_diff_GEPJ(int* dados, int tam)
{
  //Utiliza-se um vetor de 3 elementos para fazer as operaçãos de RGB dentro de um mesmo laço
  int DC[3] = {0,0,0};
  int AC[3] = {0,0,0};
  int aux[3] = {0,0,0};
  for(int i = 0; i < tam; i += 64){
    aux[0] = dados[i];
    dados[i] -= DC[0];
    DC[0] = aux[0];

    aux[1] = dados[i + tam];
    dados[i + tam] -= DC[1];
    DC[1] = aux[1];

    aux[2] = dados[i + 2 * tam];
    dados[i + 2 * tam] -= DC[2];
    DC[2] = aux[2];

    for(int j = 1; j < 64; j++){
      AC[0] = dados[i + j];
      dados[i+j] -= aux[0];
      aux[0] = AC[0];

      AC[1] = dados[i + j + tam];
      dados[i + j + tam] -= aux[1];
      aux[1] = AC[1];

      AC[2] = dados[i + j + 2 * tam];
      dados[i + j + 2 * tam] -= aux[2];
      aux[2] = AC[2];
    }
  }
}

void icod_diff_GEPJ(int* dados, int tam)
{
  //Utiliza-se um vetor de 3 elementos para fazer as operaçãos de RGB dentro de um mesmo laço
  int DC[3] = {0,0,0};
  int AC[3] = {0,0,0};
  for(int i = 0; i < tam; i += 64){
    dados[i] += DC[0];
    DC[0] = dados[i];
    AC[0] = DC[0];

    dados[i + tam] += DC[1];
    DC[1] = dados[i + tam];
    AC[1] = DC[1];

    dados[i + 2 * tam] += DC[2];
    DC[2] = dados[i + 2 * tam];
    AC[2] = DC[2];
    
    for(int j = 1; j < 64; j++){
      dados[i+j] += AC[0];
      AC[0] = dados[i + j];

      dados[i + j + tam] -= AC[1];
      AC[1] = dados[i + j + tam];

      dados[i + j + 2 * tam] -= AC[2];
      AC[2] = dados[i + j + 2 * tam];
    }
  }
}