#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitmap.h"

void dct(int* matrix);
void idct(int* matrix);
void quant(int* matrix);
void iquant(int* matrix);
void cod_diff_JPEG(int* dados, BITMAPINFOHEADER InfoHeader);
void icod_diff_JPEG(int* dados, BITMAPINFOHEADER InfoHeader);
void cod_jpeg(PIXEL *Image, BITMAPINFOHEADER InfoHeader, int* dados);
void decod_JPEG(PIXEL *Image, BITMAPINFOHEADER InfoHeader, int* dados);


float DCT_Matrix[8][8]  = {{0.354, 0.354, 0.354, 0.354, 0.354, 0.354, 0.354, 0.354},
                          {0.490, 0.416, 0.278, 0.098, -0.098, -0.278, -0.416, -0.490},
                          {0.462, 0.191, -0.191, -0.462, -0.462, -0.191, 0.191, 0.462},
                          {0.416, -0.098, -0.490, -0.278, 0.278, 0.490, 0.098, -0.416},
                          {0.354, -0.354, -0.354, 0.354, 0.354, -0.354, -0.354, 0.354},
                          {0.278, -0.490, 0.098, 0.416, -0.416, -0.098, 0.490, -0.278},
                          {0.191, -0.462, 0.462, -0.191, -0.191, 0.462, -0.462, 0.191},
                          {0.098, -0.278, 0.416, -0.490, 0.490, -0.416, 0.278, -0.098}
                          };

int QUANT_Matrix[8][8]  = {{10, 10, 15, 20, 25, 30, 35, 40},
                          {10, 15, 20, 25, 30, 35, 40, 50},
                          {15, 20, 25, 30, 35, 40, 50, 60},
                          {20, 25, 30, 35, 40, 50, 60, 70},
                          {25, 30, 35, 40, 50, 60, 70, 80},
                          {30, 35, 40, 50, 60, 70, 80, 90},
                          {35, 40, 50, 60, 70, 80, 90, 100},
                          {40, 50, 60, 70, 80, 90, 100, 110}
                          };

int ZZ_Matrix[64]       = {0, 1, 8, 16, 9, 2, 3, 10,
                          17, 24, 32, 25, 18, 11, 4, 5,
                          12, 19, 26, 33, 40, 48, 41, 34,
                          27, 20, 13, 6, 7, 14, 21, 28,
                          35, 42, 49, 56, 57, 50, 43, 36,
                          29, 22, 15, 23, 30, 37, 44, 51,
                          58, 59, 52, 45, 38, 31, 39, 46,
                          53, 60, 61, 54, 47, 55, 62, 63
                          };

int main(int argc, char *argv[])
{
  FILE *input, *cinza, *output;
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

  cod_jpeg(Image, InfoHeader, dados);

  decod_JPEG(Image, InfoHeader, dados);

  for (i=0; i < (InfoHeader.Height * InfoHeader.Width); i++){
       fputc(Image[i].B, output);
       fputc(Image[i].G, output);
       fputc(Image[i].R, output); 
    }

  fclose(output);
  return 0;
}

void cod_jpeg(PIXEL *Image, BITMAPINFOHEADER InfoHeader, int* dados)
{
  int matrix[64];

  //Para os valores de B
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    for(int j = 0; j < 64; j++) matrix[j] = Image[i+j].B;
    dct(matrix);
    quant(matrix);
    for(int j = 0; j < 64; j++) dados[i+j] = matrix[ZZ_Matrix[j]];
  }//Para os valores de G
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    for(int j = 0; j < 64; j++) matrix[j] = Image[i+j].G;
    dct(matrix);
    quant(matrix);
    for(int j = 0; j < 64; j++) dados[i + j + (InfoHeader.Height * InfoHeader.Width)] = matrix[ZZ_Matrix[j]];
  }//Para os valores de R
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    for(int j = 0; j < 64; j++) matrix[j] = Image[i+j].R;
    dct(matrix);
    quant(matrix);
    for(int j = 0; j < 64; j++) dados[i + j + (2 * InfoHeader.Height * InfoHeader.Width)] = matrix[ZZ_Matrix[j]];
  }cod_diff_JPEG(dados, InfoHeader);


  return;
}

void decod_JPEG(PIXEL *Image, BITMAPINFOHEADER InfoHeader, int* dados)
{
  int matrix[64];


  //Para os valores de B
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    for(int j = 0; j < 64; j++) matrix[ZZ_Matrix[j]] = dados[i+j];
    iquant(matrix);
    idct(matrix);
    for(int j = 0; j < 64; j++) Image[i+j].B = matrix[j];
  }//Para os valores de G
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    for(int j = 0; j < 64; j++) matrix[ZZ_Matrix[j]] = dados[i + j + (InfoHeader.Height * InfoHeader.Width)];
    iquant(matrix);
    idct(matrix);
    for(int j = 0; j < 64; j++) Image[i+j].G = matrix[j];
  }//Para os valores de R
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    for(int j = 0; j < 64; j++) matrix[ZZ_Matrix[j]] = dados[i + j + (2 * InfoHeader.Height * InfoHeader.Width)];
    iquant(matrix);
    idct(matrix);
    for(int j = 0; j < 64; j++) Image[i+j].R = matrix[j];
  }icod_diff_JPEG(dados, InfoHeader);

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

void cod_diff_JPEG(int* dados, BITMAPINFOHEADER InfoHeader)
{
  //Utiliza-se um vetor de 3 elementos para fazer as operaçãos de RGB dentro de um mesmo laço
  int DC[3] = {0,0,0};
  int AC[3] = {0,0,0};
  int aux[3] = {0,0,0};
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    aux[0] = dados[i];
    dados[i] -= DC[0];
    DC[0] = aux[0];

    aux[1] = dados[i + InfoHeader.Height * InfoHeader.Width];
    dados[i + InfoHeader.Height * InfoHeader.Width] -= DC[1];
    DC[1] = aux[1];

    aux[2] = dados[i + 2 * InfoHeader.Height * InfoHeader.Width];
    dados[i + 2 * InfoHeader.Height * InfoHeader.Width] -= DC[2];
    DC[2] = aux[2];

    for(int j = 1; j < 64; j++){
      AC[0] = dados[i + j];
      dados[i+j] -= aux[0];
      aux[0] = AC[0];

      AC[1] = dados[i + j + InfoHeader.Height * InfoHeader.Width];
      dados[i + j + InfoHeader.Height * InfoHeader.Width] -= aux[1];
      aux[1] = AC[1];

      AC[2] = dados[i + j + InfoHeader.Height * InfoHeader.Width];
      dados[i + j + 2 * InfoHeader.Height * InfoHeader.Width] -= aux[2];
      aux[2] = AC[2];
    }
  }
}

void icod_diff_JPEG(int* dados, BITMAPINFOHEADER InfoHeader)
{
  //Utiliza-se um vetor de 3 elementos para fazer as operaçãos de RGB dentro de um mesmo laço
  int DC[3] = {0,0,0};
  int AC[3] = {0,0,0};
  for(int i = 0; i < InfoHeader.Height * InfoHeader.Width; i += 64){
    dados[i] += DC[0];
    DC[0] = dados[i];
    AC[0] = DC[0];

    dados[i + InfoHeader.Height * InfoHeader.Width] += DC[1];
    DC[1] = dados[i + InfoHeader.Height * InfoHeader.Width];
    AC[1] = DC[1];

    dados[i + 2 * InfoHeader.Height * InfoHeader.Width] += DC[2];
    DC[2] = dados[i + 2 * InfoHeader.Height * InfoHeader.Width];
    AC[2] = DC[2];
    
    for(int j = 1; j < 64; j++){
      dados[i+j] += AC[0];
      AC[0] = dados[i + j];

      dados[i + j + InfoHeader.Height * InfoHeader.Width] -= AC[1];
      AC[1] = dados[i + j + InfoHeader.Height * InfoHeader.Width];

      dados[i + j + 2 * InfoHeader.Height * InfoHeader.Width] -= AC[2];
      AC[2] = dados[i + j + InfoHeader.Height * InfoHeader.Width];
    }
  }
}