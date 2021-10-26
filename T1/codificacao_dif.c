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
  unsigned int codigo;
  unsigned char tamanho;

}TABELA;

/* índice do vetor = categoria = qtd de bits; ex. categoria 1 tem 1 bit
                                	010, 011, 100, 00, 101, 110, 1110, 11110   111110*/
const unsigned char TabPrefixos[9]={2,   3,   4,   0,  5,   6,   14,   30, 	62};
const char *Palavras[9] = {"010", "011", "100", "00", "101", "110", "1110", "11110", "111110"};
const unsigned char TamPrefixos[9]={3,   3,   3,   2,  3,   3,   4,	5,  	6};

void loadBMPImage(FILE *input, BITMAPINFOHEADER InfoHeader, PIXEL *Image);
TABELA *CodDiferencial(PIXEL *Image, int altura, int largura);
void GravaBit(TABELA *TabCodigos, int tam, FILE *p);
void TonsCinza(PIXEL *Image, int tam, FILE *input);
int BitAtual = 0;

int main(int argc, char *argv[])
{
  FILE *input, *cinza, *output;
  int i;
  unsigned char *v;

  FILE *fp;
  BITMAPFILEHEADER FileHeader;   	/* File header */
  BITMAPINFOHEADER InfoHeader;   	/* Info header */
  PIXEL *Image;
  TABELA *TabCodigos;

  if(!(input = fopen(argv[1], "rb"))){
      	printf("Error: could not open input file." );
      	exit(1);
  }
printf("INFERNOoooooooooooooooooooooooooooooo");
  loadBMPHeaders (input, &FileHeader, &InfoHeader);
printf("TA TIRANDOOOOO");
  Image = (PIXEL *) malloc((InfoHeader.Width * InfoHeader.Height) * sizeof(PIXEL));

  loadBMPImage(input, InfoHeader, Image);

  TonsCinza(Image, (InfoHeader.Width * InfoHeader.Height), input);


  if(!(output = fopen("out.bin", "wb"))){
      	printf("Error: could not open ""out.bin"" file." );
      	exit(1);
  }

  fseek(input, 0, SEEK_SET);
  for(i=0; i<54; i++)
       	fputc(fgetc(input), output);
  fclose(input);

  TabCodigos = CodDiferencial(Image, InfoHeader.Height, InfoHeader.Width);
  GravaBit(TabCodigos, (InfoHeader.Height * InfoHeader.Width), output);

  fclose(output);

  return (0);
}

void TonsCinza(PIXEL *Image, int tam, FILE *input)
{
  FILE *cinza;

  for (int i=0;i< tam; i++){
 	Image[i].R = Image[i].G;
 	Image[i].B = Image[i].G;
  }

  if(!(cinza = fopen("cinza.bmp", "wb"))){
      	printf("Error: could not open ""cinza"" file." );
      	exit(1);
  }

  fseek(input, 0, SEEK_SET);

  for(int i=0; i<54; i++)
       	fputc(fgetc(input), cinza);


  for (int i=0; i < tam; i++)
  {
 	fputc(Image[i].B, cinza);
 	fputc(Image[i].G, cinza);
 	fputc(Image[i].R, cinza);

  }

  fclose (cinza);

  return;
}

TABELA *CodDiferencial(PIXEL *Image, int altura, int largura){
	int tam = altura * largura;
	TABELA *t = calloc(tam*3, sizeof(TABELA));

	t[0].codigo = Image[0].R;
	t[tam].codigo = Image[0].G;
	t[tam*2].codigo = Image[0].B;
	printf("RED:");
	for(int i = 1; i < tam; i++){
    	t[i].codigo = Image[i].R - Image[i-1].R;        	//Compara o código com a do pixel anterior
   	 //printf("codigos: %d",t[i].codigo);
    	t[i].tamanho = floor(log2(abs(t[i].codigo)) + 1);   //Calcula o número de bits necessários para representar
    //printf("tamanho: %d", t[i].tamanho);
	}
	printf("GREEN:");
	for(int i = tam+1; i < tam*2; i++){
    	t[i].codigo = Image[i-tam].G - Image[i-tam-1].G;        	//Compara o código com a do pixel anterior
    	//printf("%d",t[i].codigo);
    	t[i].tamanho = floor(log2(abs(t[i].codigo)) + 1);   //Calcula o número de bits necessários para representar
    	//printf("%d", t[i].tamanho);
	}
	//printf("BLUE:");
	for(int i = (tam*2)+1; i < tam*3; i++){
    	t[i].codigo = Image[i-(2*tam)].B - Image[i-(2*tam)-1].B;        	//Compara o código com a do pixel anterior
    	//printf("%d",t[i].codigo);
    	t[i].tamanho = floor(log2(abs(t[i].codigo)) + 1);   //Calcula o número de bits necessários para representar
    	//printf("%d", t[i].tamanho);
	}

	return t;
}




unsigned char escreve(unsigned char* palavra, int tamanhoHuff, FILE* p)
{

    unsigned char buffer[1];

	for(int i = 0; i < TamPrefixos[tamanhoHuff]+tamanhoHuff; i++){
        buffer[0] <<= 1;

    	if(palavra[i] == '1'){
        	buffer[0] |= 1;
    	}

    	BitAtual++;

    	if(BitAtual == 8){
        	fwrite(&buffer, 1, 1, p);
        	BitAtual = 0;
        	buffer[0] = 0;
    	}

	}
	return buffer[0];

}

void flush(unsigned char* buffer, FILE* p){
	while(BitAtual < 8)
	{
    	buffer[0] <<= 1;
    	BitAtual++;
	}
	fwrite(&buffer, 1, 1, p);

}

unsigned char* montaPalavra(int tamanhoHuff, int codigo){
	unsigned char* buffer = (unsigned char*)calloc(15, sizeof(unsigned char));
	unsigned char codigoBinario[8];
	itoa(codigo, codigoBinario, 2);

	for(int i = 0; i < TamPrefixos[tamanhoHuff]; i++){              	//Escrever codigo de huffman
    	buffer[i] = Palavras[tamanhoHuff][i];
	}
	if(tamanhoHuff == 8)
        printf("tam 8");
	for(int i = TamPrefixos[tamanhoHuff]; i < (TamPrefixos[tamanhoHuff]+tamanhoHuff); i++){
    	buffer[i] = codigoBinario[i - TamPrefixos[tamanhoHuff]];
	}

	return buffer;

}


void GravaBit(TABELA *TabCodigos, int tam, FILE *p){

	int a = 0;
	unsigned char remanescente[1];
    unsigned char* palavra = (unsigned char*)malloc(15);
	for(int i = 0; i < 3*tam; i++){
        if(i = (3*tam)-1)
            printf("chegou até o final");
    	palavra = montaPalavra(TabCodigos[i].tamanho, TabCodigos[i].codigo);
    	remanescente[0] = escreve(palavra, TabCodigos[i].tamanho, p);
	}
	flush(remanescente, p);


	fclose(p);

}



void loadBMPImage(FILE *input, BITMAPINFOHEADER InfoHeader, PIXEL *Image){
   int i, j, tam;

   tam = InfoHeader.Height * InfoHeader.Width;
   fseek(input, 54, SEEK_SET); //skipping the header (54 bytes)

   for (i=0; i < tam; i++){

   	Image[i].B = fgetc(input);
   	Image[i].G = fgetc(input);
   	Image[i].R = fgetc(input);
   }

}
