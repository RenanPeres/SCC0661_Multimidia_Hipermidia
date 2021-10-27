#ifndef _COD_DIF_
#define _COD_DIF_
#include "bitmap.h"

typedef struct
{
    int codigo;   //era unsigned mas mudei
    char tamanho;

}TABELA;

/* índice do vetor = categoria = qtd de bits; ex. categoria 1 tem 1 bit
                                	010, 011, 100, 00, 101, 110, 1110, 11110   111110*/
const unsigned char TabPrefixos[9]={2,   3,   4,   0,  5,   6,   14,   30, 	62};
const char *Palavras[9] = {"010", "011", "100", "00", "101", "110", "1110", "11110", "111110"};
const unsigned char TamPrefixos[9]={3,   3,   3,   2,  3,   3,   4,	5,  	6};

TABELA *CodDiferencial(PIXEL *Image, int altura, int largura);
void GravaBit(TABELA *TabCodigos, int tam, FILE *p, FILE *teste);
void TonsCinza(PIXEL *Image, int tam, FILE *input);
int BitAtual = 0;
unsigned char buffer[1];


#endif