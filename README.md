# SCC0661_Multimidia_Hipermidia

Trabalho 1 - Compressor e descompressor de imagens padrão .bmp

Leonardo Prado Dias - N°USP: 10684642
Renan Peres Martins - N°USP: 10716612

O trabalho contem dois métodos de compressão/descompressão de arquivos de imagem bitmap (.bmp) segundo as especificações contidas no escopo do trabalho.

O método 1 realiza uma compressão/descompressão sem perdas por meio de um algorítmo de codificação por diferenças e um método estatístico (codificação de Huffman);
O método 2 realiza uma compressão/descompressão com perdas  utilizando apoio de transformada DCT, quantização e ordenação Zig-Zag. Seguido de codificação por diferenças e um método estatístico (codificação de Huffman);

O usuário, quando solicitado no terminal, deve informar a forma de compressão/descompressão desejado.

Compilando:
 -> O usuário pode compilar os códigos de compressor e descompressor juntos pelo comando 'make all' no terminal;

 -> O usuário pode compilar o compressor individualmente por meio do comando 'make comprimir' no terminal;

 -> O usuário pode compilar o descompressor individualmente por meio do comando 'make descomprimir' no terminal.

Executando:
 -> O usuário pode executar a compressão de uma imagem nome_imagem.bmp, no diretório, por meio do comando './comprimir nome_imagem.bmp';

 -> O usuário pode executar a descompressão de um arquivo binario comprimido nome_arquivo.bin, no diretório, por meio do comando './descomprimir nome_arquivo.bin';