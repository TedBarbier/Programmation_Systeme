#include "fs.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
  int i;
  int bitmap_inode_tmp[10] = {1, 1, 1, 1, 0, 1, 0, 1, 0, 0};
  int bitmap_blocs_tmp[10] = {1, 1, 0, 1, 1, 1, 1, 1, 0, 0};

  /* D�but du mod�le */

  // Recopie des deux tables de bitmaps
  for (i = 0; i < 10; i++) {
    bitmap_inode[i] = bitmap_inode_tmp[i];
    bitmap_blocs[i] = bitmap_blocs_tmp[i];
  }

  // Remplissage de la table des i-noeuds
  table[0].type = 0;
  table[1].type = 0;
  table[2].type = 1;
  table[3].type = 0;
  table[5].type = 1;
  table[7].type = 1;
  table[0].taille = TMAX;
  table[1].taille = TMAX;
  table[2].taille = TMAX + 5;
  table[3].taille = TMAX;
  table[5].taille = 16;
  table[7].taille = 20;
  table[0].nb_liens = 0;
  table[1].nb_liens = 1;
  table[2].nb_liens = 1;
  table[3].nb_liens = 2;
  table[5].nb_liens = 1;
  table[7].nb_liens = 1;
  table[0].num_bloc = (int *)calloc(2, sizeof(int));
  *(table[0].num_bloc) = 1;
  *(table[0].num_bloc + 1) = 0;
  table[1].num_bloc = (int *)calloc(2, sizeof(int));
  *(table[1].num_bloc) = 1;
  *(table[1].num_bloc + 1) = 1;
  table[2].num_bloc = (int *)calloc(3, sizeof(int));
  *(table[2].num_bloc) = 2;
  *(table[2].num_bloc + 1) = 3;
  *(table[2].num_bloc + 2) = 5;
  table[3].num_bloc = (int *)calloc(2, sizeof(int));
  *(table[3].num_bloc) = 1;
  *(table[3].num_bloc + 1) = 4;
  table[5].num_bloc = (int *)calloc(2, sizeof(int));
  *(table[5].num_bloc) = 1;
  *(table[5].num_bloc + 1) = 7;
  table[7].num_bloc = (int *)calloc(2, sizeof(int));
  *(table[7].num_bloc) = 1;
  *(table[7].num_bloc + 1) = 6;

  // Remplissage des donn�es des blocs
  strcpy(blocs[0], ". 0\n.. 0\ntoto 1\ntata 3\n");
  strcpy(blocs[1], ". 1\n.. 0\na.txt 2\nb.txt 5\n");
  for (i = 0; i < TMAX - 1; i++) {
    strcpy(blocs[3] + i, "a");
  }
  strcpy(blocs[4], ". 3\n.. 0\nc.txt 7\n");
  for (i = 0; i < 20; i++) {
    strcpy(blocs[5] + i, "b");
  }
  strcpy(blocs[6], "123456789067890\n");
  strcpy(blocs[7], "bonjour\nbonjour\n");

  /* Fin du mod�le */

  // Fonctions � impl�menter:
  void print(int num_inoeud) {
    for (int i = 1; i <= table[num_inoeud].num_bloc[0]; i++) {
      printf("%s", blocs[table[num_inoeud].num_bloc[i]]);
    }
  }
  int get_num_inode(int num_inode_root, char * name) {
    //printf("get_num_inode(%i, %s)\n", num_inode_root, name);
    for (int i = 1; i <= table[num_inode_root].num_bloc[0]; i++) {
      printf("%s\n", blocs[table[num_inode_root].num_bloc[i]]);
      if (strcmp(blocs[table[num_inode_root].num_bloc[i]], name)) {
        char * bloc = fscanf("%s %i",blocs[table[num_inode_root].num_bloc[i]]); 
        return * (bloc + 1);
      } else if (table[num_inode_root].type == 0) {
        return get_num_inode(table[num_inode_root].num_bloc[i], name);
      }
    }
    return -1;
  }
  // Ecrit le contenu du fichier de l'i-noeud 2 � l'�cran
  print(2);
  printf("\n");

  // Recup�rer un num�ro d'i-noeud pour un nom de fichier
  // dans un r�pertoire dont on donne le num�ro d'i-noeud
  int numero_inoeud = get_num_inode(1, "a.txt");
  printf("Numéro de l'i-noeud pour a.txt dans le répertoire d'i-noeud 1: %i\n",
         numero_inoeud);
}
