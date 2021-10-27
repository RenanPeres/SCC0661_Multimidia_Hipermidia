#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bitmap.h"

typedef struct
{
    	unsigned char R;
    	unsigned char G;
    	unsigned char B;
}PIXEL;

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



void decodHuffman(TABELA* t, unsigned char byte){
    unsigned char buffer;
    int tamHuffman;

    buffer = byte & 0xC0;           //Armazena os dois primeiros digitos

    while(1){
        if(buffer == 0){
            tamHuffman = 3;
            break;
        }

        buffer = byte & 0xE0;       //Armazena os tres primeiros digitos
        if(buffer == 0x40){         //010
            tamHuffman = 0;
            break;
        }else if(buffer == 0x60){   //011
            tamHuffman = 1;
            break;
        }else if(buffer == 0x80){   //100
            tamHuffman = 2;
            break;
        }else if(buffer == 0xA0){   //101
            tamHuffman = 4;
            break;
        }else if(buffer == 0xC0){   //110
            tamHuffman = 5;
            break;
        }

        buffer = byte & 0xF0;       //Armazena os 4 primeiros digitos

        if(buffer == 0xE0){
            tamHuffman = 6;
            break;
        }

        buffer = byte & 0xF8;   //Armazena os 5 primeiros digitos

        if(buffer == 0xF0){     //11110
            tamHuffman = 7;
            break;
        }

        buffer = byte & 0xFC;   //Armazena os 6 primeiros digitos

        if(buffer == 0xF8){     //111110
            tamHuffman = 8;
            break;
        }

    }

}