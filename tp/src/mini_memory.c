/**
 * @file mini_memory.c
 * @brief Custom memory allocation functions.
 *
 * This file contains the implementation of a custom memory allocation function
 * using the `sbrk` system call. It includes the definition of a structure to
 * keep track of allocated memory blocks and a function to allocate and
 * initialize memory blocks.
 *
 * @include <unistd.h>
 * @include <stdio.h>
 * @include <string.h>
 * @include <stdlib.h>
 * @include "mini_lib.h"
 */

/**
 * @struct malloc_element
 * @brief Structure to keep track of allocated memory blocks.
 *
 * This structure is used to keep track of memory blocks allocated by the
 * custom memory allocation function. It contains a pointer to the allocated
 * memory, the size of the allocated memory, the status of the memory block
 * (free or used), and a pointer to the next malloc_element in the list.
 *
 * @var malloc_element::ptr
 * Pointer to the allocated memory.
 * @var malloc_element::total_size
 * Size of the allocated memory (size_element * number_element).
 * @var malloc_element::state
 * Status of the memory block (0: free, 1: used).
 * @var malloc_element::next_malloc
 * Pointer to the next malloc_element (NULL if none).
 */

/**
 * @var malloc_list
 * @brief Head of the linked list of malloc_element structures.
 *
 * This variable points to the first element in the linked list of
 * malloc_element structures that keep track of allocated memory blocks.
 */

/**
 * @brief Allocates and initializes memory blocks.
 *
 * This function allocates memory for a specified number of elements of a
 * specified size using the `sbrk` system call. It initializes the allocated
 * memory to zero and keeps track of the allocated memory block using a
 * malloc_element structure.
 *
 * @param size_element Size of each element to be allocated.
 * @param number_element Number of elements to be allocated.
 * @return Pointer to the allocated memory, or NULL if the allocation fails.
 */
// include standard libraries
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// include personal library
#include "mini_lib.h"
// Memory allocation function

struct malloc_element {
    void *ptr;               // Pointer to the allocated memory
    int total_size;          // Size of the allocated memory (size_element * number_element)
    int state;               // Status of the memory block (0: free, 1: used)
    struct malloc_element *next_malloc; // Pointer to the next malloc_element (NULL if none)
};

struct malloc_element *malloc_list = NULL;

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
