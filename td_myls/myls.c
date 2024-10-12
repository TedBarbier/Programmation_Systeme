#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

struct stat info;

char **ls(char *file) {  // Correction : paramètre 'char *' au lieu de 'char'
  DIR *dir;
  struct dirent *entry;
  char **file_list = NULL;
  int file_count = 0;

  if ((dir = opendir(file)) == NULL) {
    perror("opendir");
    return NULL;
  }

  while ((entry = readdir(dir)) != NULL) {
    if ((strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))) {
      file_list = realloc(file_list, sizeof(char *) * (file_count + 1));
      file_list[file_count] = strdup(entry->d_name);
      file_count++;
    }
  }
  closedir(dir);

  // Add a NULL pointer at the end of the list
  file_list = realloc(file_list, sizeof(char *) * (file_count + 1));
  file_list[file_count] = NULL;

  return file_list;
}


void print_ls(char **list) {
  int i = 0;
  while (list[i] != NULL) {
    printf("%s ", list[i]);
    free(list[i]); // Free each string after printing
    i++;
  }
  printf("\n");
  free(list); // Free the list itself
}

char *DROITS(mode_t mode){
  char *droits = malloc(11);
  sprintf(droits, "%c%c%c%c%c%c%c%c%c%c",
    S_ISDIR(mode) ? 'd' : 
    S_ISLNK(mode) ? 'l' : 
    S_ISCHR(mode) ? 'c' : 
    S_ISBLK(mode) ? 'b' : 
    S_ISFIFO(mode) ? 'p' : 
    S_ISSOCK(mode) ? 's' : '-',
    mode & S_IRUSR ? 'r' : '-',
    mode & S_IWUSR ? 'w' : '-',
    mode & S_IXUSR ? 'x' : '-',
    mode & S_IRGRP ? 'r' : '-',
    mode & S_IWGRP ? 'w' : '-',
    mode & S_IXGRP ? 'x' : '-',
    mode & S_IROTH ? 'r' : '-',
    mode & S_IWOTH ? 'w' : '-',
    mode & S_IXOTH ? 'x' : '-'
  );
  return droits;
}

int ls_l(char *file) {
  struct stat info;
  stat(file, &info);
  printf("%s ", DROITS(info.st_mode));
  printf("%li ", info.st_nlink);
  struct passwd *pw = getpwuid(info.st_uid);
  printf("%s ", pw ? pw->pw_name : "unknown");
  struct group *gr = getgrgid(info.st_gid);
  printf("%s ", gr ? gr->gr_name : "unknown");
  printf("%li ", info.st_size);
  char timebuf[80];
  strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&info.st_mtime));
  printf("%s ", timebuf);
  printf("%s\n", file);
  return 0;
}

int ls_l_list(char **list_files) {
    int i = 0;
    while (list_files[i] != NULL) {
        ls_l(list_files[i]);  // Appelle ls_l pour chaque fichier
        i++;
    }
  return 0;
}

int ls_l_recursive(char *path) {
    struct stat info;
    char **file_list = ls(path);  // Récupère la liste des fichiers dans ce répertoire
    ls_l_list(file_list);  // Affiche les informations avec l'option -l

    // Boucle sur chaque fichier du répertoire
    int i = 0;
    while (file_list[i] != NULL) {
        char new_path[PATH_MAX];
        snprintf(new_path, sizeof(new_path), "%s/%s", path, file_list[i]);  // Crée le chemin complet
        stat(new_path, &info);

        // Si c'est un répertoire et pas "." ni "..", on fait un appel récursif
        if (S_ISDIR(info.st_mode) && strcmp(file_list[i], ".") && strcmp(file_list[i], "..")) {
            printf("\n%s:\n", new_path);  // Affiche le nom du sous-répertoire
            ls_l_recursive(new_path);     // Appel récursif
        }
        i++;
    }

    // Libérer la mémoire allouée pour la liste
    i = 0;
    while (file_list[i] != NULL) {
        free(file_list[i]);
        i++;
    }
    free(file_list);
    return 0;
}

int main(int argc, char **argv) {
  if (argc == 1) {
    print_ls(ls("."));
  } else {
    char **list_args = malloc(sizeof(char *) * argc);
    char **list_files = malloc(sizeof(char *) * argc);
    int args = 0, files = 0;
    int argl = 0, argR = 0;
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
        switch (argv[i][1]) {
          case 'l':
            argl = 1;
            break;
          case 'R':
            argR = 1;
            break;
          default:
            break;
        }
        list_args[args++] = strdup(argv[i]);
      } else {
        list_files[files++] = strdup(argv[i]);
      }
    }
    list_args[args] = NULL;
    list_files[files] = NULL;
    
    if (argl == 0 && argR == 0 && files == 0) {
      print_ls(ls("."));
    } else if (argl == 0 && argR == 0 && files >= 1) {
      print_ls(list_files);
    } else if (argl == 1 && argR == 0 && files == 0) {
      ls_l_list(ls("."));  // Appelle la fonction qui gère ls -l pour chaque fichier
    } else if (argl == 1 && argR == 0 && files >= 1) {
      ls_l_list(list_files);  // Utilisation de la fonction normale d'affichage
    } else if (argl == 1 && argR == 1 && files == 0) {
      ls_l_recursive(".");  // Appel récursif avec ls -l et -R
    } else if (argl == 1 && argR == 1 && files >= 1) {
      for (int i = 0; i < files; i++) {
        ls_l_recursive(list_files[i]);
      }
    } 
  }

  return 0;
}
