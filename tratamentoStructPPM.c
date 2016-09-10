#include <stdio.h>
#include <stdlib.h>

FILE *loadImage(char *filename){
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Erro ao tentar carregar o arquivo!");
        exit(-1);
    }
    return fp;
}

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

//to do: Colocar ; no final do arquivo
void createStruct(char *nome,FILE *fp,int widthImg, int heightImg) {
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
