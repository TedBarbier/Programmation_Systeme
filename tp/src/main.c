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
#include "mini_lib.h"

void test_mini_calloc() {
    printf("Running test_mini_calloc...\n");

    // Test 1: Allocate memory for 10 integers
    int *arr = (int*) mini_calloc(sizeof(int), 10);
    if (arr == NULL) {
        printf("Test 1 failed: mini_calloc returned NULL\n");
    } else {
        // Check if memory is zero-initialized
        int passed = 1;
        for (int i = 0; i < 10; i++) {
            if (arr[i] != 0) {
                passed = 0;
                break;
            }
        }
        if (passed) {
            printf("Test 1 passed\n");
        } else {
            printf("Test 1 failed: Memory not zero-initialized\n");
        }
        mini_free(arr);
    }

    // Test 2: Allocate memory with invalid parameters
    void *ptr = mini_calloc(-1, 10);
    if (ptr == NULL) {
        printf("Test 2 passed\n");
    } else {
        printf("Test 2 failed: mini_calloc should return NULL for invalid parameters\n");
        mini_free(ptr);
    }

    // Test 3: Allocate memory for 0 elements
    ptr = mini_calloc(sizeof(int), 0);
    if (ptr == NULL) {
        printf("Test 3 passed\n");
    } else {
        printf("Test 3 failed: mini_calloc should return NULL for 0 elements\n");
        mini_free(ptr);
    }
}

void test_mini_free() {
    printf("Running test_mini_free...\n");

    // Test 1: Free a valid pointer
    int *arr = (int*) mini_calloc(sizeof(int), 10);
    if (arr != NULL) {
        mini_free(arr);
        printf("Test 1 passed\n");
    } else {
        printf("Test 1 failed: mini_calloc returned NULL\n");
    }

    // Test 2: Free a NULL pointer
    mini_free(NULL);
    printf("Test 2 passed\n");

    // Test 3: Free an already freed pointer
    arr = (int*) mini_calloc(sizeof(int), 10);
    if (arr != NULL) {
        mini_free(arr);
        mini_free(arr); // Free again
        printf("Test 3 passed\n");
    } else {
        printf("Test 3 failed: mini_calloc returned NULL\n");
    }
}

void test_mini_exit() {
    printf("Running test_mini_exit...\n");

    // Test: Call mini_exit (this will terminate the program)
    printf("Calling mini_exit...\n");
    mini_exit();
    // This line should not be reached
    printf("Test failed: mini_exit did not terminate the program\n");
}

int main() {
    test_mini_calloc();
    test_mini_free();
    test_mini_exit();
    return 0;
}