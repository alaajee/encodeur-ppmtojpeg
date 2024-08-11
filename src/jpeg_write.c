#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "dct2.c"
#include "acdc.c"
#include "read_image.c"

void appx(FILE* jpeg){
    uint8_t application_data[]={0xff, 0xd8, 0xff, 0xe0, 00, 0x10, 0x4A, 0x46, 0x49, 0x46, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0};
    fwrite(&application_data, sizeof(application_data), 1, jpeg);
}

void entete_DQT(FILE* jpeg,ImageInfo info){
    uint8_t entete[]={0xff, 0xdb, 0, 67, 0};
    fwrite(&entete, sizeof(entete), 1 , jpeg );
    fwrite(&quantification_table_Y , sizeof(quantification_table_Y), 1 , jpeg );
    if (strcmp(info.type, "P6") == 0){
        uint8_t entete1[]={0xff, 0xdb, 0, 67, 01};
        fwrite(&entete1, sizeof(entete1), 1 , jpeg );
        fwrite(&quantification_table_CbCr , sizeof(quantification_table_CbCr), 1 , jpeg );
    }
}

void entete_DHT(FILE* jpeg){
    uint8_t marqueurdc[]={0xff, 0xc4,0x00,0x1F,0x00};
    fwrite(&marqueurdc, sizeof(marqueurdc), 1, jpeg);
    fwrite(htables_nb_symb_per_lengths[0][0], sizeof(htables_nb_symb_per_lengths[0][0]), 1, jpeg);
    fwrite(htables_symbols[0][0],htables_nb_symbols[0][0], 1, jpeg);
    uint8_t marqueurac[]={0xff, 0xc4,0x00,0xb5,0x10};
    fwrite(&marqueurac, sizeof(marqueurac), 1, jpeg);
    fwrite(htables_nb_symb_per_lengths[1][0], sizeof(htables_nb_symb_per_lengths[1][0]), 1, jpeg);    
    fwrite(htables_symbols[1][0], htables_nb_symbols[1][0], 1, jpeg);
}

void entete_SOF(FILE* jpeg, ImageInfo info , int h1,int v1, int h2,int v2, int h3,int v3){
    uint8_t hauteur = info.longueur / 256;
    uint8_t hauteur1 = info.longueur % 256;
    uint8_t largeur = info.largeur / 256;
    uint8_t largeur1 = info.largeur % 256;  
    if (strcmp(info.type, "P6") == 0){
        uint8_t sof[] = {0xff, 0xc0, 0x00, 0x11,0x08, hauteur, hauteur1, largeur, largeur1, 0x03};
        fwrite(&sof, sizeof(sof), 1, jpeg);
        uint8_t ss[] = {0x01, (h1 << 4) | (v1 & 0x0F), 0x00};
        fwrite(&ss, sizeof(ss), 1, jpeg);
        uint8_t sf[] = {0x02, (h2 << 4) | (v2 & 0x0F), 0x01};
        fwrite(&sf, sizeof(sf), 1, jpeg);
        uint8_t sss[] = {0x03, (h3 << 4) | (v3 & 0x0F), 0x01};
        fwrite(&sss, sizeof(sss), 1, jpeg);
    }
    else {
        uint8_t sof[] = {0xff, 0xc0, 0x00, 0x0b,0x08, hauteur, hauteur1, largeur, largeur1, 0x01, 0x01, 0x11, 0x00};
        fwrite(&sof, sizeof(sof), 1, jpeg);
    }
}
