Exercice 4.
```c
/**
 * @file mini_memory.c
 * @brief Memory allocation functions for the mini_lib library.
 *
 * This file contains the implementation of a custom memory allocation function
 * that allocates and initializes memory to zero.
 *
 * @include <unistd.h>
 * @include <stdio.h>
 * @include <string.h>
 * @include <stdlib.h>
 * @include "mini_lib.h"
 */

/**
 * @brief Allocates memory for an array of elements and initializes it to zero.
 *
 * This function allocates memory for an array of `number_element` elements,
 * each of size `size_element` bytes, and initializes the allocated memory to zero.
 *
 * @param size_element The size of each element in bytes.
 * @param number_element The number of elements to allocate.
 * @return A pointer to the allocated memory, or NULL if the allocation fails.
 */
// include standard libraries
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// include personal library
#include "mini_lib.h"
// Memory allocation function
void* mini_calloc(int size_element, int number_element) {
    // parameter validation
    if (size_element <= 0 || number_element <= 0) {
        return NULL;
    }
    // Memory allocation
    void *memoire = sbrk(size_element * number_element);
    if (memoire == (void*) -1) {
        perror("sbrk");
        return NULL; // sbrk failed
    }

    // Initialize allocated memory to zero
    memset(memoire, 0, size_element * number_element);
    return memoire;
}

```

Exercice 5.
Initialiser le buffer avec des '\0' avant de le renvoyer permet de s'assurer que la mémoire allouée est propre et ne contient pas de données résiduelles. Cela évite des comportements indéfinis ou des failles de sécurité potentielles dues à l'utilisation de mémoire non initialisée. En d'autres termes, cela garantit que toutes les valeurs dans le bloc de mémoire sont initialisées à zéro, ce qui est souvent le comportement attendu lors de l'allocation de mémoire pour des structures de données.

Exercice 6.
La fonction `free` en C est utilisée pour libérer la mémoire précédemment allouée dynamiquement à l'aide des fonctions `malloc`, `calloc` ou `realloc`. Lorsqu'on appelle `free` sur un pointeur, la mémoire pointée par ce pointeur est marquée comme libre et peut être réutilisée par de futures allocations. Cependant, `free` ne remet pas nécessairement cette mémoire au système d'exploitation immédiatement. Elle la rend disponible pour l'allocateur de mémoire du programme, qui peut la réutiliser pour des allocations futures.

Il est important de noter que `free` ne modifie pas la valeur du pointeur passé en argument. Après avoir libéré la mémoire, il est recommandé de mettre le pointeur à `NULL` pour éviter les accès à une mémoire déjà libérée, ce qui pourrait entraîner des comportements indéfinis.

exercice 7.
```c
struct malloc_element {
    void *ptr;               // Pointer to the allocated memory
    int total_size;          // Size of the allocated memory (size_element * number_element)
    int state;               // Status of the memory block (0: free, 1: used)
    struct malloc_element *next_malloc; // Pointer to the next malloc_element (NULL if none)
};
```

Exercice 8.
```c
struct malloc_element *malloc_list = NULL;
```

Exercice 9.
```c 
void* mini_calloc(int size_element, int number_element) {
    // parameter validation
    if (size_element <= 0 || number_element <= 0) {
        return NULL;
    }
    // Memory allocation
    void *memory = sbrk(size_element * number_element);
    if (memory == (void*) -1) {
        perror("sbrk");
        return NULL; // sbrk failed
    }

    // Initialize allocated memory to zero
    memset(memory, 0, size_element * number_element);

    // Create a new malloc_element
    struct malloc_element *new_element = (struct malloc_element*) sbrk(sizeof(struct malloc_element));
    if (new_element == (void*) -1) {
        perror("sbrk");
        return NULL; // sbrk failed
    }

    // Initialize the new malloc_element
    new_element->ptr = memory;
    new_element->total_size = size_element * number_element;
    new_element->state = 1; // Mark as used
    new_element->next_malloc = NULL;

    // Add the new element to the malloc_list
    if (malloc_list == NULL) {
        malloc_list = new_element;
    } else {
        struct malloc_element *current = malloc_list;
        while (current->next_malloc != NULL) {
            current = current->next_malloc;
        }
        current->next_malloc = new_element;
    }

    return memory;
}

Exercice 10.
```c
void mini_free(void *ptr) {
    // parameter validation
    if (ptr == NULL) {
        return;
    }

    // Find the malloc_element corresponding to the pointer
    struct malloc_element *current = malloc_list;
    struct malloc_element *previous = NULL;
    while (current != NULL && current->ptr != ptr) {
        previous = current;
        current = current->next_malloc;
    }

    // If the pointer is not found, return
    if (current == NULL) {
        return;
    }

    // Mark the memory block as free
    current->state = 0;

    // Check if the previous memory block is also free
    if (previous != NULL && previous->state == 0) {
        // Merge the two memory blocks
        previous->total_size += current->total_size;
        previous->next_malloc = current->next_malloc;
        brk(current);
    }

    // Check if the next memory block is also free
    if (current->next_malloc != NULL && current->next_malloc->state == 0) {
        // Merge the two memory blocks
        current->total_size += current->next_malloc->total_size;
        current->next_malloc = current->next_malloc->next_malloc;
        brk(current->next_malloc);
    }
}
```

Exercice 11.
```c 
void* mini_calloc(int size_element, int number_element) {
    // parameter validation
    if (size_element <= 0 || number_element <= 0) {
        return NULL;
    }

    int total_size = size_element * number_element;

    // Check for a free block that can be reused
    struct malloc_element *current = malloc_list;
    struct malloc_element *previous = NULL;
    while (current != NULL) {
        if (current->state == 0 && current->total_size >= total_size) {
            // Reuse this block
            current->state = 1;
            memset(current->ptr, 0, total_size);
            return current->ptr;
        }
        previous = current;
        current = current->next_malloc;
    }

    // No suitable free block found, allocate new memory
    void *memory = sbrk(total_size);
    if (memory == (void*) -1) {
        perror("sbrk");
        return NULL; // sbrk failed
    }

    // Initialize allocated memory to zero
    memset(memory, 0, total_size);

    // Create a new malloc_element
    struct malloc_element *new_element = (struct malloc_element*) sbrk(sizeof(struct malloc_element));
    if (new_element == (void*) -1) {
        perror("sbrk");
        return NULL; // sbrk failed
    }

    // Initialize the new malloc_element
    new_element->ptr = memory;
    new_element->total_size = total_size;
    new_element->state = 1; // Mark as used
    new_element->next_malloc = NULL;

    // Add the new element to the malloc_list
    if (malloc_list == NULL) {
        malloc_list = new_element;
    } else {
        previous->next_malloc = new_element;
    }

    return memory;
}
```