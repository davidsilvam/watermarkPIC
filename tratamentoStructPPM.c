/**
 * @file tratamentoStructPPM.c
 *
 * @author Bruno Macabeus, David Silva
 *
 * @version 0.1
 *
 * @copyright
 * Copyright (C) Parallax, Inc. 2012. All Rights MIT Licensed.
 *
 * @brief Gera o valor a ser usado nas variáveis globais mapPixelsDest e mapPixelsSour do main.c
 *
 * Para gerar os valores, é necessário usar imagens no formato PPM versão 6.
 *
 * Abrirá no working directory a imagem chamada de "destination.ppm" e então gerará o arquivo
 * "outputStructDestination.txt" com o conteúdo a ser usada na mapPixelsDest.
 * De forma análoga abrirá a imagem chamada "source.ppm" para então gerar o arquivo "outputStructSource.txt"
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Abre um arquivo com rb. Nesse código, ele é usado para abrir as imagens PPM 6
 * @param filename Nome do arquivo a ser aberto
 * @return Ponteiro para o arquivo aberto. Em caso de erro, o programa é encerrado.
 */
FILE *loadImage(char *filename){
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Erro ao tentar carregar o arquivo!");
        exit(-1);
    }
    return fp;
}

/**
 * Ler as dimensões do arquivo PPM
 * @param fp Ponteiro para o arquivo PPM
 * @param widthImg Variável de saída. Nela será armazenada a largura da imagem
 * @param heightImg Variável de saída. Nela será armazenado o tamanho da imagem
 */
void readDimension(FILE *fp, int *widthImg, int *heightImg) {
    int i;
    char textBuffer[4];
    for (i = 0; i < 4; i++) {
        fread(textBuffer + i, sizeof(char), 1, fp);
        if (textBuffer[i] == ' ') {
            break;
        }
    }
    if (i == 4) {
        printf("Erro: Imagem com dimensões não suportadas!");
        return;
    }
    textBuffer[i] = '\0';
    *widthImg = atoi(textBuffer);
    for (i = 0; i < 4; i++) {
        fread(textBuffer + i, sizeof(char), 1, fp);
        if (textBuffer[i] == '\n') {
            break;
        }
    }
    if (i == 4) {
        printf("Erro: Imagem com dimensões não suportadas!");
        return;
    }
    textBuffer[i] = '\0';

    *heightImg = atoi(textBuffer);
}

/**
 * Cria um arquivo de texto com uma array do mapa de pixels da imagem PPM
 * @param nome Vetor de chars com o nome do arquivo que será salvo
 * @param fp Ponteiro para o arquivo PPM
 * @param widthImg Largura da imagem
 * @param heightImg Altura da imagem
 */
void createStruct(char *nome, FILE *fp, int widthImg, int heightImg) {
    int i, j,z,r;
    char str[3];
    FILE *fpStr;
    fpStr = fopen(nome, "w");
    if(fpStr == NULL) printf("Erro ao criar arquivo!");
    fputc('{',fpStr);
    for (i = 0;i < heightImg;i++) {
        fputc('{',fpStr);
        for(j = 0;j < widthImg;j++){
            fputc('{',fpStr);
            for(z = 0;z < 3;z++){
                fread(&r, sizeof(char), 1, fp);
                sprintf(str,"%d",r);
                fprintf(fpStr,"%d",atoi(str));
                if(z != 2)
                    putc(',',fpStr);
            }
            fputc('}',fpStr);
            if(j != heightImg-1)
                fputc(',',fpStr);
        }
        fputc('}',fpStr);
        if(i != heightImg - 1)
            fputc(',',fpStr);
    }
    fputc('}',fpStr);
    fputc(';',fpStr);
    fclose(fpStr);
}

int main(){
    int widthImg,heightImg;
    FILE *fp;

    fp = loadImage("destination.ppm");

    fseek(fp, 3, SEEK_CUR);
    readDimension(fp, &widthImg, &heightImg);
    fseek(fp, 4, SEEK_CUR);
    createStruct("outputStructDestination.txt",fp, widthImg, heightImg);

    fp = loadImage("source.ppm");
    fseek(fp, 3, SEEK_CUR);
    readDimension(fp, &widthImg, &heightImg);
    fseek(fp, 4, SEEK_CUR);
    createStruct("outputStructSource.txt",fp, widthImg, heightImg);

    fclose(fp);

    return 0;
}
