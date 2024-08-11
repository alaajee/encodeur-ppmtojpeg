#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/read_image.h"

/// Fonction pour ecrire l'entete application 
void appx(FILE* jpeg);

/// Fonction pour ecrire l'entete et les tables de quantification
void entete_DQT(FILE* jpeg,ImageInfo info);

/// Fonction pour ecrire l'entete et les tables de huffman
void entete_DHT(FILE* jpeg,ImageInfo info);

/// Fonction pour ecrire l'entete SOF en tenant compte des facteurs de sous-echantillonnage
void entete_SOF(FILE* jpeg, ImageInfo info , int h1,int v1, int h2,int v2, int h3,int v3);
