//Trabalho 1 - Multimídia e Hipermídia
//Leonardo Prado Dias - N°USP: 10684642
//Renan Peres Martins - N°USP: 10716612
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "codificacao_dif.h"

int BitAtual = 0;
/* índice do vetor = categoria = qtd de bits; ex. categoria 1 tem 1 bit
                                	010, 011, 100, 00, 101, 110, 1110, 11110   111110*/
const unsigned char TabPrefixos[9]={2,   3,   4,   0,  5,   6,   14,   30, 	62};
const char *Palavras[9] = {"010", "011", "100", "00", "101", "110", "1110", "11110", "111110"};
const unsigned char TamPrefixos[9]={3,   3,   3,   2,  3,   3,   4,	5,  	6};

/* CodDiferencial() -> Função principal da codificação por diferença
*PIXEL *Image -> Recebe uma struct PIXEL com os dados de leitura do .bmp a ser comprimido
*int altura -> Recebe a altura da imagem .bmp lida
*int largura -> Recebe a largura da imagem .bmp lida
*Return t -> Retorna uma struct do tipo TABELA
*/
TABELA *CodDiferencial(PIXEL *Image, int altura, int largura){
	int tam = altura * largura;
	TABELA *t = calloc(tam*3, sizeof(TABELA));

	t[0].codigo = Image[0].R;
	t[tam].codigo = Image[0].G;
	t[tam*2].codigo = Image[0].B;
	for(int i = 1; i < tam; i++){
    	t[i].codigo = Image[i].R - Image[i-1].R;        	//Compara o código com a do pixel anterior
   	 //printf("codigos: %d",t[i].codigo);
    	t[i].tamanho = floor(log2(abs(t[i].codigo)) + 1);   //Calcula o número de bits necessários para representar
    //printf("tamanho: %d", t[i].tamanho);
	}
	for(int i = tam+1; i < tam*2; i++){
    	t[i].codigo = Image[i-tam].G - Image[i-tam-1].G;        	//Compara o código com a do pixel anterior
    	//printf("%d",t[i].codigo);
    	t[i].tamanho = floor(log2(abs(t[i].codigo)) + 1);   //Calcula o número de bits necessários para representar
    	//printf("%d", t[i].tamanho);
	}
	for(int i = (tam*2)+1; i < tam*3; i++){
    	t[i].codigo = Image[i-(2*tam)].B - Image[i-(2*tam)-1].B;        	//Compara o código com a do pixel anterior
    	//printf("%d",t[i].codigo);
    	t[i].tamanho = floor(log2(abs(t[i].codigo)) + 1);   //Calcula o número de bits necessários para representar
    	//printf("%d", t[i].tamanho);
	}

	return t;
}

/* Decod_GEPJ() -> Função principal da decodificação com perdas utilizada
*TABELA *TabCodigos ->Recebe um struct TABELA com os dados do .bin a ser descomprimidos
*int altura -> Recebe a altura da imagem .bmp lida
*int largura -> Recebe a largura da imagem .bmp lida
*Return i -> Retorna uma struct do tipo PIXEL
*/
PIXEL *DecodDiferencial(TABELA *TabCodigos, int altura, int largura){
	int tam = altura * largura;
	PIXEL *i = calloc(tam, sizeof(PIXEL));

	i[0].R = TabCodigos[0].codigo;
  i[0].G = TabCodigos[tam].codigo;
  i[0].B = TabCodigos[tam * 2].codigo;
	for(int j = 1; j < tam; j++){
        i[j].R = (i[j-1].R + TabCodigos[j].codigo);
        i[j].G = (i[j-1].G + TabCodigos[j+tam].codigo);
        i[j].B = (i[j-1].B + TabCodigos[j+2*tam].codigo);
	}

	return i;
}

/* escreve() -> Escreve a palavra no arquivo .bin
*unsigned char* palavra -> Palavra a ser escrita no .bin
*int tamanhoHuff -> Tamanho do código pela codificação Huffman
*FILE* p -> Ponteiro para o arquivo de saída .bin
*Return buffer[0] -> 
*/
unsigned char escreve(unsigned char* palavra, int tamanhoHuff, FILE* p)
{

	for(int i = 0; i < TamPrefixos[tamanhoHuff]+tamanhoHuff; i++){
        buffer[0] <<= 1;
        if(palavra[i] == 1){
            buffer[0] |= 0x1;
        }
        BitAtual++;

            if(BitAtual == 8){
                fwrite(buffer, 1, 1, p);
                BitAtual = 0;
                buffer[0] = 0;
    	}


    }
	return buffer[0];

}

/* flush() -> Completa o bits necessário para completar um byte
*unsigned char* buffer -> Bits restantes para serem escritos
*FILE* p -> Arquivo de saída .bin
*/
void flush(unsigned char* buffer, FILE* p){
	while(BitAtual < 8)
	{
    	buffer[0] <<= 1;
    	BitAtual++;
	}
	fwrite(&buffer, 1, 1, p);
}

/* onverterIntEmChar() -> Função que converte um valor inteiro em sua representação em vetor de char
*int n -> Valor do código a ser convertido
*Return bin -> Retorna a sequência de char referente a n
*/
char* converterIntEmChar(int n){
    char* bin = (char*)calloc(9, sizeof(char));
    int nMod = abs(n);
    int i = 0;

    while (nMod > 0) {

        bin[i] = nMod % 2;
        nMod = nMod / 2;
        i++;
    }
    bin[i] = '\0';

    if(n < 0){     //Faz complemento de 1
        for(int j = 0; j < 8; j++){
            bin[j] = !bin[j];
        }
    }

    return bin;
}

/* montaPalavra() -> Monta a palavra para escrever em arquivo
*int tamanhoHuff -> Tamanho do código
*int codigo -> Código
*Return buffer -> Retorna o valor de 'sobra' do bit
*/
unsigned char* montaPalavra(int tamanhoHuff, int codigo){
	unsigned char* buffer = (unsigned char*)calloc(15, sizeof(unsigned char));
	char* codigoBinario = (char*)calloc(8, sizeof(char));
	codigoBinario = converterIntEmChar(codigo);


	for(int i = 0; i < TamPrefixos[tamanhoHuff]; i++){              	//Escrever codigo de huffman
    	buffer[i] = Palavras[tamanhoHuff][i];
    	if(buffer[i] == '1')
            buffer[i] = 1;
        else
            buffer[i] = 0;
	}

	int i = TamPrefixos[tamanhoHuff];
	while((codigoBinario[i - TamPrefixos[tamanhoHuff]] != '\0') && (i < (TamPrefixos[tamanhoHuff]+tamanhoHuff))){
        buffer[i] = codigoBinario[i - TamPrefixos[tamanhoHuff]];
        i++;
	}
	buffer[i] = '\0';


/*	for(int i = TamPrefixos[tamanhoHuff]; i < (TamPrefixos[tamanhoHuff]+tamanhoHuff); i++){
        if(codigoBinario[i - TamPrefixos[tamanhoHuff]] != '\0')
            buffer[i] = codigoBinario[i - TamPrefixos[tamanhoHuff]];
        else
	}*/

	return buffer;

}

/* GravaBit() -> Função que converte a lista de códigos em bits e escreve o .bin
*TABELA *TabCodigos -> Ponteiro para struct TABELA que possui as informações auxiliares dos códigos
*int tam -> Tamanho de pixel do .bmp
*FILE *p -> Ponteiro para o arquivo de saida .bin
*/
void GravaBit(TABELA *TabCodigos, int tam, FILE *p){
	int a = 0;
	unsigned char remanescente[1];
    unsigned char* palavra = (unsigned char*)malloc(15);
	for(int i = 0; i < 3*tam; i++){
    	palavra = montaPalavra(TabCodigos[i].tamanho, TabCodigos[i].codigo);
    	remanescente[0] = escreve(palavra, TabCodigos[i].tamanho, p);
	}
	flush(remanescente, p);

}

TABELA *LeituraBin(TABELA *TabCodigos, FILE *p)
{
    int codHuffman;
    int i = 0;
    int tamanho;
		fseek(p, 55, SEEK_SET);
    codHuffman = read_bit(p);		
    while(codHuffman != EOF){
        tamanho = TamHuffman(p, codHuffman);
				if(tamanho != 0){
					codHuffman = read_bit(p);
					if(codHuffman == 1){
						if(tamanho == 1) TabCodigos[i].codigo = 1;
						else if(tamanho == 2) TabCodigos[i].codigo = codHuffman*2+read_bit(p);
						else if(tamanho == 3) TabCodigos[i].codigo = codHuffman*4+read_bit(p)*2+read_bit(p);
						else if(tamanho == 4) TabCodigos[i].codigo = codHuffman*8+read_bit(p)*4+read_bit(p)*2+read_bit(p);
						else if(tamanho == 5) TabCodigos[i].codigo = codHuffman*16+read_bit(p)*8+read_bit(p)*4+read_bit(p)*2+read_bit(p);
						else if(tamanho == 6) TabCodigos[i].codigo = codHuffman*32+read_bit(p)*16+read_bit(p)*8+read_bit(p)*4+read_bit(p)*2+read_bit(p);
						else if(tamanho == 7) TabCodigos[i].codigo = codHuffman*64+read_bit(p)*32+read_bit(p)*16+read_bit(p)*8+read_bit(p)*4+read_bit(p)*2+read_bit(p);
						else TabCodigos[i].codigo = codHuffman*128+read_bit(p)*64+read_bit(p)*32+read_bit(p)*16+read_bit(p)*8+read_bit(p)*4+read_bit(p)*2+read_bit(p);
					}else{if(tamanho == 1) TabCodigos[i].codigo = -1;
						else if(tamanho == 2) TabCodigos[i].codigo = -1*((!codHuffman)*2+(read_bit(p)==0));
						else if(tamanho == 3) TabCodigos[i].codigo = -1*((!codHuffman)*4+(read_bit(p)==0)*2+(read_bit(p)==0));
						else if(tamanho == 4) TabCodigos[i].codigo = -1*((!codHuffman)*8+(read_bit(p)==0)*4+(read_bit(p)==0)*2+(read_bit(p)==0));
						else if(tamanho == 5) TabCodigos[i].codigo = -1*((!codHuffman)*16+(read_bit(p)==0)*8+(read_bit(p)==0)*4+(read_bit(p)==0)*2+(read_bit(p)==0));
						else if(tamanho == 6) TabCodigos[i].codigo = -1*((!codHuffman)*32+(read_bit(p)==0)*16+(read_bit(p)==0)*8+(read_bit(p)==0)*4+(read_bit(p)==0)*2+(read_bit(p)==0));
						else if(tamanho == 7) TabCodigos[i].codigo = -1*((!codHuffman)*64+(read_bit(p)==0)*32+(read_bit(p)==0)*16+(read_bit(p)==0)*8+(read_bit(p)==0)*4+(read_bit(p)==0)*2+(read_bit(p)==0));
						else TabCodigos[i].codigo = -1*((!codHuffman)*128+(read_bit(p)==0)*64+(read_bit(p)==0)*32+(read_bit(p)==0)*16+(read_bit(p)==0)*8+(read_bit(p)==0)*4+(read_bit(p)==0)*2+(read_bit(p)==0));
					}
				}else TabCodigos[i].codigo = 0;
				i++;
        codHuffman = read_bit(p);
    }
    return TabCodigos;
}

int read_bit(FILE* f){
	static int calls=0;
	static int acc;
	static int first = 1;
	if(first==1){
		acc=fgetc(f);
		first=0;
	}
	if(acc==EOF){
		return EOF;
	}
	if(calls==8){
		acc=fgetc(f);
		if(acc==EOF){
			return EOF;
		}
		calls=0;
	}
	int offset = 7 - calls;
	int mask = 1 << offset; 
	int bit = ((acc & mask)>>offset);	
	calls++;
	return bit;
}

int TamHuffman(FILE* p, int ini)
{
    if(ini == 0){//0x
        if(read_bit(p) == 0) return 3;//00
        if(read_bit(p) == 0) return 0;//010
        return 1;//011
    }if(read_bit(p)==0){//10x
        if(read_bit(p)==0) return 2;//100
        return 4;//101
    }if(read_bit(p)==0) return 5;//110
    if(read_bit(p)==0) return 6;//1110
    if(read_bit(p)==0) return 7;//11110
    if(read_bit(p)==0) return 8;//111110
}