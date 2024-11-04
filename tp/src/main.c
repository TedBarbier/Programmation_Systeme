/**
 * @file main.c
 * @brief This file contains test functions for the mini_calloc and mini_free functions.
 *
 * The tests include:
 * - Allocating memory and checking if it's initialized to zero.
 * - Allocating memory with invalid parameters.
 * - Reusing a free block.
 * - Freeing a valid pointer.
 * - Freeing a NULL pointer.
 * - Freeing a pointer not allocated by mini_calloc.
 *
 * The main function runs all the tests.
 */

/**
 * @brief Tests the mini_calloc function.
 *
 * This function runs the following tests:
 * - Test 1: Allocates memory and checks if it's initialized to zero.
 * - Test 2: Allocates memory with invalid parameters.
 * - Test 3: Reuses a free block.
 */
void test_mini_calloc();

/**
 * @brief Tests the mini_free function.
 *
 * This function runs the following tests:
 * - Test 1: Frees a valid pointer and checks if the block is marked as free.
 * - Test 2: Frees a NULL pointer.
 * - Test 3: Frees a pointer not allocated by mini_calloc.
 */
void test_mini_free();

/**
 * @brief Tests the mini_exit function.
 *
 * This function runs the following test:
 * - Test 1: Ensures mini_exit terminates the program.
 */
void test_mini_exit();

/**
 * @brief Main function to run all tests.
 *
 * This function calls test_mini_calloc, test_mini_free, and test_mini_exit to run all the tests.
 * 
 * @return int Returns 0 on successful execution.
 */
int main();

// include standard libraries
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// include personal library
#include "mini_memory.c"

void test_mini_calloc() {
    printf("Running test_mini_calloc...\n");

    // Test 1: Allocate memory and check if it's initialized to zero
    int *arr = (int*) mini_calloc(sizeof(int), 5);
    if (arr == NULL) {
        printf("Test 1 Failed: mini_calloc returned NULL\n");
        return;
    }
    for (int i = 0; i < 5; i++) {
        if (arr[i] != 0) {
            printf("Test 1 Failed: Memory not initialized to zero\n");
            return;
        }
    }
    printf("Test 1 Passed\n");

    // Test 2: Allocate memory with invalid parameters
    void *ptr = mini_calloc(-1, 5);
    if (ptr != NULL) {

        printf("Test 2 Failed: mini_calloc should return NULL for invalid parameters\n");
        return;
    }
    printf("Test 2 Passed\n");

    // Test 3: Reuse a free block
    mini_free(arr);
    int *arr2 = (int*) mini_calloc(sizeof(int), 5);
    if (arr2 != arr) {
        printf("Test 3 Failed: mini_calloc did not reuse the free block\n");
        return;
    }
    printf("Test 3 Passed\n");
}

void test_mini_free() {
    printf("Running test_mini_free...\n");

    // Test 1: Free a valid pointer
    int *arr = (int*) mini_calloc(sizeof(int), 5);
    mini_free(arr);

    // Parcourir malloc_list pour vérifier si le bloc est bien libre
    struct malloc_element *current = malloc_list;
    int found = 0;
    while (current != NULL) {
        if (current->ptr == arr) {
            found = 1;
            if (current->state != 0) {
                printf("Test 1 Failed: mini_free did not mark the block as free\n");
                return;
            }
            break;
        }
        current = current->next_malloc;
    }
    if (!found) {
        printf("Test 1 Failed: Block not found in malloc_list\n");
        return;
    }

    printf("Test 1 Passed\n");

    // Test 2: Free a NULL pointer
    mini_free(NULL);
    printf("Test 2 Passed\n"); // Should not crash

    // Test 3: Free a pointer not allocated by mini_calloc
    int dummy;
    mini_free(&dummy);
    printf("Test 3 Passed\n"); // Should not crash
}



void test_mini_exit() {
    printf("Running test_mini_exit...\n");

    // Test 1: Ensure mini_exit terminates the program
    // Note: This test will terminate the program, so it should be the last test
    printf("Test 1: mini_exit should terminate the program\n");
    mini_exit();
    printf("Test 1 Failed: mini_exit did not terminate the program\n");
}

int main() {
    test_mini_calloc();
    test_mini_free();
    test_mini_exit();
    return 0;
}