#ifndef _COD_GEPJ_
#define _COD_GEPJ_
#include "codificacao_dif.h"
        
void dct(int* matrix);
void idct(int* matrix);
void quant(int* matrix);
void iquant(int* matrix);
TABELA *Cod_GEPJ(PIXEL *Image, BITMAPINFOHEADER InfoHeader);
PIXEL *Decod_GEPJ(TABELA *TabCodigos, BITMAPINFOHEADER InfoHeader);


#endif