/**
 * @file mini_memory.c
 * @brief Custom memory allocation and deallocation functions.
 *
 * This file contains implementations of custom memory allocation and deallocation
 * functions using a linked list to manage memory blocks. It includes functions
 * for allocating zero-initialized memory blocks and freeing allocated memory.
 *
 * @author Ted
 * @date 2024-11-14
 */

/*
 * @struct malloc_element
 * @brief Structure representing a memory block.
 *
 * This structure is used to represent a memory block in the custom memory
 * allocation system. It contains information about the allocated memory,
 * its size, state (free or used), and a pointer to the next memory block.
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
 * @brief Head of the linked list of memory blocks.
 *
 * This variable points to the first element in the linked list of memory blocks.
 * It is used to manage the memory blocks allocated by the custom memory allocation
 * functions.
 */

 /**
 * @brief Allocates zero-initialized memory for an array.
 *
 * This function allocates memory for an array of elements, initializes the memory
 * to zero, and returns a pointer to the allocated memory. If a suitable free block
 * is found, it reuses the block; otherwise, it allocates new memory.
 *
 * @param size_element Size of each element.
 * @param number_element Number of elements.
 * @return Pointer to the allocated memory, or NULL if allocation fails.
 */
void* mini_calloc(int size_element, int number_element);

 /**
 * @brief Frees the allocated memory.
 *
 * This function frees the memory block pointed to by the given pointer. It marks
 * the memory block as free and attempts to merge adjacent free blocks.
 *
 * @param ptr Pointer to the memory block to be freed.
 */
void mini_free(void *ptr);

 /**
 * @brief Exits the program.
 *
 * This function terminates the program with an exit status of 0.
 */
void mini_exit();


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

void* mini_memset(void *ptr, int value, int num) {
    // parameter validation
    if (ptr == NULL || num < 0) {
        return NULL;
    }

    char *p = (char*) ptr;
    for (int i = 0; i < num; i++) {
        p[i] = value;
    }

    return ptr;
}

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
       write(2,"sbrk",4);
        return NULL; // sbrk failed
    }

    // Initialize allocated memory to zero
    mini_memset(memory, 0, total_size);

    // Create a new malloc_element
    struct malloc_element *new_element = sbrk(sizeof(struct malloc_element));
    if (new_element == (void*) -1) {
        write(2,"sbrk",4);
        // Free the previously allocated memory to avoid memory leak
        sbrk(-total_size);
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

void mini_free(void* ptr) {
    if (ptr == NULL) {
        printf("mini_free: NULL pointer, nothing to free.\n");
        return; // Do nothing if the pointer is NULL
    }

    struct malloc_element *current = malloc_list;

    // Traverse the list to find the element corresponding to the pointer
    while (current != NULL) {
        if (current->ptr == ptr) {
            // Check if the block is already marked as used
            if (current->state == 1) {
                // Mark the block as free
                current->state = 0;
            } else {
                printf("mini_free: Block at %p is already free.\n", ptr); // Message if the block is already free
            }
            return; // Exit after finding and freeing the memory
        }
        current = current->next_malloc;
    }

    // If we reach this point, it means the pointer was not found
    write(2, "mini_free: Error, pointer not allocated by mini_calloc\n", 54);
}

void mini_exit()
{
    mini_exit_printf();
    _exit(0);
}