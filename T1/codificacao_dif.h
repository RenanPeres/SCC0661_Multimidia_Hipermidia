//Trabalho 1 - Multimídia e Hipermídia
//Leonardo Prado Dias - N°USP: 10684642
//Renan Peres Martins - N°USP: 10716612
#ifndef _COD_DIF_
#define _COD_DIF_
#include "bitmap.h"

/*Struct que armazena as informações para codificação Huffman*/
typedef struct
{
    int codigo;
    char tamanho;

}TABELA;

/* CodDiferencial() -> Função principal da codificação por diferença*/
TABELA *CodDiferencial(PIXEL *Image, int altura, int largura);

/* Decod_GEPJ() -> Função principal da decodificação com perdas utilizada*/
PIXEL *DecodDiferencial(TABELA *TabCodigos, int altura, int largura);

/* GravaBit() -> Função que converte a lista de códigos em bits e escreve o .bin*/
void GravaBit(TABELA *TabCodigos, int tam, FILE *p);

TABELA *LeituraBin(TABELA *TabCodigos, FILE *p);

int read_bit(FILE* f);

int TamHuffman(FILE* p, int ini);

unsigned char buffer[1];


#endif