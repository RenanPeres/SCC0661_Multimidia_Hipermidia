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
        int valor;
        unsigned int codigo;
        unsigned char tamanho;
  
}TABELA;

/* índice do vetor = categoria = qtd de bits; ex. categoria 1 tem 1 bit
                                    010, 011, 100, 00, 101, 110, 1110, 11110   111110*/
const unsigned char TabPrefixos[9]={2,   3,   4,   0,  5,   6,   14,   30,     62};
const unsigned char TamPrefixos[9]={3,   3,   3,   2,  3,   3,   4,    5,      6};

void loadBMPImage(FILE *input, BITMAPINFOHEADER InfoHeader, PIXEL *Image);
TABELA *CodDiferencial(PIXEL *Image, int altura, int largura);
void GravaBit(TABELA *TabCodigos, int tam, FILE *p);
void TonsCinza(PIXEL *Image, int tam, FILE *input);

       
int cod_dif()
{
  FILE *input, *cinza, *output;
  int i;
  unsigned char *v;
  
  FILE *fp;
  BITMAPFILEHEADER FileHeader;       /* File header */
  BITMAPINFOHEADER InfoHeader;       /* Info header */
  PIXEL *Image;
  TABELA *TabCodigos;
         
  loadBMPHeaders (input, &FileHeader, &InfoHeader);
  
  Image = (PIXEL *) malloc((InfoHeader.Width * InfoHeader.Height) * sizeof(PIXEL));
  
  loadBMPImage(input, InfoHeader, Image);
                               

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
        

        Image[0].R =;
        for (int i=0;i< tam; i++){
                Image[i].R = ;
  }
 
}

void write_huffman(TABELA *TabCodigos, int tam, FILE *p)
{
 
}

void printBit(char *bitstream, FILE *out)
{
    unsigned char buffer = 0; /* ocupa um byte de espaço de memória*/
    int i;
  
    
     /*===> AQUI <======
      Código para transferir o conteúdo do bitstream para um byte, no caso, para 
      a variável buffer.
     */
       
       
    printf("\n%d", buffer);

    out = fopen("out.bin","wb"); /* grava o bitstream como bits -> 8 bits*/
    fwrite(&buffer,1,sizeof(buffer), out);
    fclose(out);
    
    printf("\n");
}