#define TMAX 100

typedef struct inode {
  int type; // Type du fichier: 0=rep 1=fichier
  int taille; // Taille en octets
  int nb_liens; // Nombre de lien
  int * num_bloc; // Table des num�ros de blocs
} inode;


int bitmap_inode[10];
int bitmap_blocs[10];
char blocs[10][TMAX];
inode table[10];
