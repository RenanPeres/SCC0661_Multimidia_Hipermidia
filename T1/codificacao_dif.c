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
        i[j].R = i[j-1].R + TabCodigos[j].codigo;
        i[j].G = i[j-1].G + TabCodigos[j+tam].codigo;
        i[j].B = i[j-1].B + TabCodigos[j+2*tam].codigo;
	}

	return i;
}

void arquivoTeste(unsigned char caracter, FILE* teste){
    unsigned char* convertido = (unsigned char*)malloc(9);
    char palavra[8];

	for(int i=0;i<8;i++)
	{
		palavra[i]=caracter%2;
		caracter=caracter/2;
	}


    for(int i = 0; i < 8; i++){
        if(palavra[i] == 0)
            convertido[i] = '0';
        else
            convertido[i] = '1';
    }
    convertido[8] = '\0';
    fprintf(teste, "%s\n", convertido);
    return;

}

/* escreve() ->
*unsigned char* palavra ->
*int tamanhoHuff -> 
*FILE* p ->
*FILE* teste ->
*Return buffer[0] ->
*/
unsigned char escreve(unsigned char* palavra, int tamanhoHuff, FILE* p, FILE* teste)
{

/*	for(int i = 0; i < TamPrefixos[tamanhoHuff]+tamanhoHuff; i++){
        buffer[0] <<= 1;

    	if(palavra[i] == 1){
        	buffer[0] |= 1;
    	}

    	BitAtual++;

    	if(BitAtual == 8){
        	fwrite(buffer, 1, 1, p);
        	arquivoTeste(buffer[0], teste);
        	BitAtual = 0;
        	buffer[0] = 0;
    	}

	}*/

	for(int i = 0; i < TamPrefixos[tamanhoHuff]+tamanhoHuff; i++){
        buffer[0] >>= 1;
        if(palavra[i] == 1){
            buffer[0] |= 0x80;
        }
        BitAtual++;

            if(BitAtual == 8){
                fwrite(buffer, 1, 1, p);
                arquivoTeste(buffer[0], teste);
                BitAtual = 0;
                buffer[0] = 0;
    	}


    }
	return buffer[0];

}

/* flush() ->
*unsigned char* buffer ->
*FILE* p ->
*/
void flush(unsigned char* buffer, FILE* p){
	while(BitAtual < 8)
	{
    	buffer[0] >>= 1;
    	BitAtual++;
	}
	fwrite(&buffer, 1, 1, p);
}

/* onverterIntEmChar() -> 
*int n ->
*Return bin ->
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

/* montaPalavra() ->
*int tamanhoHuff ->
*int codigo ->
*Return buffer ->
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

/* GravaBit() ->
*TABELA *TabCodigos ->
*int tam ->
*FILE *p ->
*/
void GravaBit(TABELA *TabCodigos, int tam, FILE *p, FILE* teste){
	int a = 0;
	unsigned char remanescente[1];
    unsigned char* palavra = (unsigned char*)malloc(15);
	for(int i = 0; i < 3*tam; i++){
    	palavra = montaPalavra(TabCodigos[i].tamanho, TabCodigos[i].codigo);
    	remanescente[0] = escreve(palavra, TabCodigos[i].tamanho, p, teste);
	}
	flush(remanescente, p);

}

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

TABELA* lerHuffman(FILE* input){
    TABELA* t;
    unsigned char buffer;

    fseek(input, 0x37, SEEK_SET);    //Pula o header do arquivo

    while(buffer != EOF){
        fread(&buffer, sizeof(unsigned char), 1, input);
        decodHuffman(t, buffer);
    }

    return t;
}
