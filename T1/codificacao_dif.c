#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "codificacao_dif.h"
#include "bitmap.h"

int main(int argc, char *argv[])
{
  FILE *input, *cinza, *output;
  int i;
  unsigned char *v;
FILE* teste;
teste = fopen("teste.txt", "w");
  FILE *fp;
  BITMAPFILEHEADER FileHeader;   	/* File header */
  BITMAPINFOHEADER InfoHeader;   	/* Info header */
  PIXEL *Image;
  TABELA *TabCodigos;

  if(!(input = fopen(argv[1], "rb"))){
      	printf("Error: could not open input file." );
      	exit(1);
  }
  loadBMPHeaders (input, &FileHeader, &InfoHeader);

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
  GravaBit(TabCodigos, (InfoHeader.Height * InfoHeader.Width), output, teste);

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

void flush(unsigned char* buffer, FILE* p){
	while(BitAtual < 8)
	{
    	buffer[0] >>= 1;
    	BitAtual++;
	}
	fwrite(&buffer, 1, 1, p);
}

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

void GravaBit(TABELA *TabCodigos, int tam, FILE *p, FILE* teste){
	int a = 0;
	unsigned char remanescente[1];
    unsigned char* palavra = (unsigned char*)malloc(15);
	for(int i = 0; i < 3*tam; i++){
    	palavra = montaPalavra(TabCodigos[i].tamanho, TabCodigos[i].codigo);
    	remanescente[0] = escreve(palavra, TabCodigos[i].tamanho, p, teste);
	}
	flush(remanescente, p);

    fclose(teste);
	fclose(p);

}