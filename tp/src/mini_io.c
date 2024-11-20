

#include <fcntl.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "mini_lib.h"

#define IOBUFFER_SIZE 2048

#define MAX_FILES 10 // Définir le nombre maximum de fichiers ouverts simultanément

typedef struct OpenFile {
    MYFILE* file;
    struct OpenFile* next;
} OpenFile;

static OpenFile* open_files = NULL; // Liste des fichiers ouverts

void add_open_file(MYFILE* file) {
    OpenFile* new_entry = (OpenFile*)mini_calloc(sizeof(OpenFile), 1);
    if (new_entry) {
        new_entry->file = file;
        new_entry->next = open_files;
        open_files = new_entry;
    }
}
void remove_open_file(MYFILE* file) {
    OpenFile** current = &open_files;
    while (*current) {
        if ((*current)->file == file) {
            OpenFile* to_delete = *current;
            *current = to_delete->next;
            mini_free(to_delete);
            return;
        }
        current = &(*current)->next;
    }
}





MYFILE* mini_fopen(char* file, char mode) {
    // Vérification si le nom du fichier est valide
    if (file == NULL) {
        errno = EINVAL;
        return NULL;
    }

    // Allocation de mémoire pour le fichier
    MYFILE* File = (MYFILE*)mini_calloc(sizeof(MYFILE), 1);
    if (File == NULL) {
        errno = ENOMEM;  // Mémoire insuffisante
        return NULL;
    }

    // Initialisation des buffers et indices
    File->buffer_read = NULL;
    File->buffer_write = NULL;
    File->ind_read = -1;
    File->ind_write = -1;

    // Définition des flags d'ouverture du fichier en fonction du mode
    int flags;
    switch (mode) {
        case 'r':
            flags = O_RDONLY;
            break;
        case 'w':
            flags = O_WRONLY | O_CREAT | O_TRUNC;
            break;
        case 'b':
            flags = O_RDWR | O_CREAT;
            break;
        case 'a':
            flags = O_WRONLY | O_CREAT | O_APPEND;
            break;
        default:
            mini_free(File);
            errno = EINVAL;  // Mode non valide
            return NULL;
    }

    // Ouverture du fichier
    File->fd = open(file, flags, 0664);  // Permissions 0664
    if (File->fd == -1) {
        // Si l'ouverture échoue, libère la mémoire et retourne NULL
        mini_free(File);
        errno = errno;  // Conserve l'erreur d'open
        return NULL;
    }

    // Ajout du fichier à la liste des fichiers ouverts
    add_open_file(File);

    return File;
}


void* mini_memcpy(void* dest, const void* src, int n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;

    for (int i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}

void* mini_memmove(void* dest, const void* src, int n) {
    // Vérification des pointeurs
    if (dest == NULL || src == NULL || n < 0) {
        return NULL;  // Retourne NULL si les entrées sont invalides
    }

    char* d = (char*)dest;
    const char* s = (const char*)src;

    // Si la destination est avant la source (les blocs ne se chevauchent pas)
    if (d < s) {
        // Copier de gauche à droite
        for (int i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else if (d > s) {
        // Si la destination est après la source (les blocs ne se chevauchent pas)
        // Copier de droite à gauche
        for (int i = n - 1; i >= 0; i--) {
            d[i] = s[i];
        }
    }

    // Si la destination et la source se chevauchent, rien à faire (cas déjà couvert par les deux premiers cas)
    return dest;
}



int mini_fread(void* buffer, int size_element, int number_element, MYFILE* file) {
    if (!buffer || !file || size_element <= 0 || number_element <= 0) {
        errno = EINVAL; // Paramètres invalides
        mini_perror("Invalid parameters");
        return 0;
    }

    int total_size = size_element * number_element; // Taille totale à lire
    int bytes_read = 0;  // Nombre total de caractères lus
    char* user_buffer = (char*)buffer;

    // Allocation du tampon de lecture si nécessaire
    if (!file->buffer_read) {
        file->buffer_read = mini_calloc(IOBUFFER_SIZE, 1);
        if (!file->buffer_read) {
            errno = ENOMEM;
            mini_perror("Failed to allocate buffer_read");
            return 0;
        }
        file->ind_read = 0; // Initialisation de l'indicateur
    }

    while (bytes_read < total_size) {
        // Si le tampon est vide, lire un nouveau bloc depuis le fichier
        if (file->ind_read <= 0) {
            int result = read(file->fd, file->buffer_read, IOBUFFER_SIZE);
            if (result == 0) {
                // Fin de fichier atteinte
                break;
            } else if (result < 0) {
                // Gestion des erreurs de lecture
                mini_perror("Error reading file");
                return bytes_read; // Retourne ce qui a été lu avant l'erreur
            }
            file->ind_read = result; // Met à jour la taille de données dans le tampon
        }

        // Calculer combien de données copier du tampon
        int bytes_to_copy = total_size - bytes_read;
        if (bytes_to_copy > file->ind_read) {
            bytes_to_copy = file->ind_read;
        }

        // Copier les données du tampon vers le buffer utilisateur
        mini_memcpy(user_buffer + bytes_read, file->buffer_read, bytes_to_copy);

        // Mettre à jour les compteurs et les indices
        bytes_read += bytes_to_copy;
        file->ind_read -= bytes_to_copy;

        // Décaler les données restantes dans le tampon
        if (file->ind_read > 0) {
            mini_memmove(file->buffer_read, 
                         (char*)file->buffer_read + bytes_to_copy, 
                         file->ind_read);
        }
    }

    return bytes_read; // Retourne le nombre de caractères lus
}


int mini_fwrite(void* buffer, int size_element, int number_element, MYFILE* file) {
    if (!buffer || !file || size_element <= 0 || number_element <= 0) {
        errno = EINVAL; // Paramètres invalides
        return -1;
    }

    int total_size = size_element * number_element; // Taille totale à écrire
    int bytes_written = 0; // Nombre total d'octets effectivement écrits
    char* user_buffer = (char*)buffer;

    // Allocation du tampon d'écriture si nécessaire
    if (!file->buffer_write) {
        file->buffer_write = mini_calloc(IOBUFFER_SIZE, 1);
        if (!file->buffer_write) {
            errno = ENOMEM; // Échec d'allocation mémoire
            return -1;
        }
        file->ind_write = 0; // Initialisation de l'indice d'écriture
    }

    while (bytes_written < total_size) {
        // Calcul de l'espace disponible dans le tampon
        int available_space = IOBUFFER_SIZE - file->ind_write;

        // Calculer combien d'octets on peut écrire dans le tampon
        int bytes_to_copy = total_size - bytes_written;
        if (bytes_to_copy > available_space) {
            bytes_to_copy = available_space;
        }

        // Copier les données dans le tampon d'écriture
        mini_memcpy((char*)file->buffer_write + file->ind_write, 
                    user_buffer + bytes_written, 
                    bytes_to_copy);

        file->ind_write += bytes_to_copy;
        bytes_written += bytes_to_copy;

        // Si le tampon est plein, déclencher une écriture
        if (file->ind_write == IOBUFFER_SIZE) {
            int result = write(file->fd, file->buffer_write, IOBUFFER_SIZE);
            if (result == -1) {
                mini_perror("Error writing to file");
                return -1; // Échec d'écriture
            }
            file->ind_write = 0; // Réinitialiser l'indicateur d'écriture
        }
    }

    return bytes_written; // Retourne le nombre d'octets écrits
}


int mini_fflush(MYFILE* file) {
    if (!file || !file->buffer_write || file->ind_write <= 0) {
        // Aucun fichier valide ou rien à écrire
        return 0;
    }

    // Écrire les données restantes du tampon dans le fichier
    int result = write(file->fd, file->buffer_write, file->ind_write);
    if (result == -1) {
        mini_perror("Error flushing buffer");
        return -1; // Erreur lors de l'écriture
    }

    // Réinitialiser l'indicateur d'écriture après une écriture réussie
    file->ind_write = 0;

    return result; // Retourne le nombre d'octets écrits
}

int mini_fclose(MYFILE* file) {
    if (!file) return -1;

    // Flusher les données restantes
    if (file->buffer_write && file->ind_write > 0) {
        if (mini_fflush(file) == -1) {
            return -1;
        }
    }

    // Fermer le fichier
    if (file->fd != -1) {
        close(file->fd);
    }

    // Libérer les tampons et la structure
    if (file->buffer_read) {
        mini_free(file->buffer_read);
    }
    if (file->buffer_write) {
        mini_free(file->buffer_write);
    }
    remove_open_file(file); // Retirer de la liste des fichiers ouverts
    mini_free(file);

    return 0;
}

void mini_exit_flush() {
    OpenFile* current = open_files;  // Parcours de la liste des fichiers ouverts

    while (current) {
        MYFILE* file = current->file;

        // Si le tampon d'écriture contient des données non écrites, les flusher
        if (file && file->buffer_write && file->ind_write > 0) {
            int result = mini_fflush(file);  // Flusher le tampon d'écriture
            if (result == -1) {
                mini_perror("Error flushing buffer on exit");
            }
        }

        // Passer au fichier suivant dans la liste
        current = current->next;
    }
}


