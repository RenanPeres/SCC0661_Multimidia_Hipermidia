#Makefile - Trabalho 1 - Multimídia e Hipermídia
#Leonardo Prado Dias - N°USP: 10684642
#Renan Peres Martins - N°USP: 10716612

all: comprimir descomprimir

comprimir: compressor.o bitmap.o codificacao_dif.o codificacao_GEPJ.o
	gcc -o comprimir compressor.o bitmap.o codificacao_dif.o codificacao_GEPJ.o -lm

descomprimir: descompressor.o bitmap.o codificacao_dif.o codificacao_GEPJ.o
	gcc -o descomprimir descompressor.o bitmap.o codificacao_dif.o codificacao_GEPJ.o -lm

compressor.o: compressor.c codificacao_GEPJ.h
	gcc -o compressor.o compressor.c -c

descompressor.o: descompressor.c codificacao_GEPJ.h
	gcc -o descompressor.o descompressor.c -c

bitmap.o: bitmap.c bitmap.h
	gcc -o bitmap.o bitmap.c -c

codificacao_dif.o: codificacao_dif.c codificacao_dif.h
	gcc -o codificacao_dif.o codificacao_dif.c -c -lm

codificacao_GEPJ.o: codificacao_GEPJ.c codificacao_GEPJ.h
	gcc -o codificacao_GEPJ.o codificacao_GEPJ.c -c

clean:
	rm -rf *.o *~ comprimir *~ descomprimir