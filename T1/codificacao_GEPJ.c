#include <stdio.h>
#include <stdlib.h>
#include "codificacao_GEPJ.h"

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

TABELA *Cod_GEPJ(PIXEL *Image, BITMAPINFOHEADER InfoHeader)
{
  int matrix[64];
  TABELA *Tabela;
  

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

  return Tabela;
}

PIXEL *Decod_GEPJ(TABELA *TabCodigos, BITMAPINFOHEADER InfoHeader)
{
  int matrix[64];
  PIXEL *Image;

  Image = DecodDiferencial(TabCodigos,InfoHeader.Height,InfoHeader.Width);
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