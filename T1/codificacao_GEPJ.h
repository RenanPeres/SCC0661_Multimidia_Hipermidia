//Trabalho 1 - Multimídia e Hipermídia
//Leonardo Prado Dias - N°USP: 10684642
//Renan Peres Martins - N°USP: 10716612
#ifndef _COD_GEPJ_
#define _COD_GEPJ_
#include "codificacao_dif.h"

/* Cod_GEPJ() -> Função principal da codificação com perdas utilizada*/
TABELA *Cod_GEPJ(PIXEL *Image, BITMAPINFOHEADER InfoHeader);

/* Decod_GEPJ() -> Função principal da decodificação com perdas utilizada*/
PIXEL *Decod_GEPJ(TABELA *TabCodigos, BITMAPINFOHEADER InfoHeader);

/* dct() -> Função que realiza a transformada DCT*/
void dct(int* matrix);

/* idct() -> Função que realiza a transformada DCT inversa*/
void idct(int* matrix);

/* quant() -> Função que realiza a quantização dos dados*/
void quant(int* matrix);

/* iquant() -> Função que realiza a inversa da quantização dos dados*/
void iquant(int* matrix);


#endif