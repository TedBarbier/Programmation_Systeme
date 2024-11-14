//include standart library
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//include personal library
#include "mini_lib.h"

//define constant
#define BUF_SIZE 1024

char buffer[BUF_SIZE];
int ind = -1;

// Fonction mini_printf
void mini_printf(char *str) {
    // Parcourir la chaîne de caractères passée en argument
    if (ind == -1) {
        ind = 0;
    }
    while (*str) {
        buffer[ind++] = *str;

        // Si le tampon est plein ou si le caractère est un saut de ligne
        if (ind == BUF_SIZE || *str == '\n') {
            // Écrire le contenu du tampon dans la sortie standard
            write(STDOUT_FILENO, buffer, ind);
            ind = 0; // Réinitialiser l'indice pour reprendre à 0
        }

        str++;
    }
    ind = 0;
}

