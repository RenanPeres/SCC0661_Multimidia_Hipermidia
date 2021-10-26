#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitmap.h"

void dct(int* matrix);
void idct(int* matrix);
void quant(int* matrix, int fator);
void iquant(int* matrix, int fator);
void cod_jpeg();

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

int main(){
  cod_jpeg();
  return 0;
}

void cod_jpeg(){

  int fator = 1;

  int matrix1[64] =  {52, 55, 61, 66, 70, 61, 64, 73,
                      63, 59, 66, 90, 109, 85, 69, 72,
                      62, 59, 68, 113, 144, 104, 66, 73,
                      63, 58, 71, 122, 154, 106, 70, 69,
                      67, 61, 68, 104, 126, 88, 68, 70,
                      79, 65, 60, 70, 77, 68, 58, 75,
                      85, 71, 64, 59, 55, 61, 65, 83,
                      87, 79, 69, 68, 65, 76, 78, 94
                    };

  int matrix[64] =  {52, 55, 61, 66, 70, 61, 64, 73,
                      63, 59, 66, 90, 109, 85, 69, 72,
                      62, 59, 68, 113, 144, 104, 66, 73,
                      63, 58, 71, 122, 154, 106, 70, 69,
                      67, 61, 68, 104, 126, 88, 68, 70,
                      79, 65, 60, 70, 77, 68, 58, 75,
                      85, 71, 64, 59, 55, 61, 65, 83,
                      87, 79, 69, 68, 65, 76, 78, 94
                    };
  dct(matrix);
  quant(matrix, fator);

  iquant(matrix, fator);
  idct(matrix);

    for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++)
        printf("%d ", matrix[i*8+j]);
      printf("\n");
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

void quant(int* matrix, int fator)
{
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      matrix[i*8+j] = (int)matrix[i*8+j]/(fator * QUANT_Matrix[i][j]);
    }
  }
}

void iquant(int* matrix, int fator)
{
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      matrix[i*8+j] *= QUANT_Matrix[i][j] * fator;
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