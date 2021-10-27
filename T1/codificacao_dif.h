#ifndef _COD_DIF_
#define _COD_DIF_
#include "bitmap.h"

typedef struct
{
    int codigo;   //era unsigned mas mudei
    char tamanho;

}TABELA;

TABELA *CodDiferencial(PIXEL *Image, int altura, int largura);
PIXEL *DecodDiferencial(TABELA *TabCodigos, int altura, int largura);
void GravaBit(TABELA *TabCodigos, int tam, FILE *p, FILE *teste);
void TonsCinza(PIXEL *Image, int tam, FILE *input);

unsigned char buffer[1];


#endif