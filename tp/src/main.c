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
#include <assert.h>


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

void test_mini_printf() {
    // Test case 1: Simple string
    char test1[] = "Hello, World!\n";
    printf("Test 1: %s\n", test1);
    mini_printf(test1);

    // Test case 2: String without newline
    char test2[] = "This is a test string without newline";
    printf("Test 2: %s\n", test2);
    mini_printf(test2);

    // Test case 3: String with multiple newlines
    char test3[] = "Line 1\nLine 2\nLine 3\n";
    printf("Test 3: %s\n", test3);
    mini_printf(test3);

    // Test case 4: String that exceeds buffer size
    char test4[1024 + 10];
    memset(test4, 'A', 1024 + 9);
    test4[1024 + 9] = '\0';
    printf("Test 4: String of length %d\n", 1024 + 9);
    mini_printf(test4);
}

void test_mini_scanf() {
    char buffer[100];
    int result;

    // Test 1: Normal input
    const char *input1 = "Hello, World!";
    write(STDIN_FILENO, input1, strlen(input1));
    result = mini_scanf(buffer, sizeof(buffer));
    assert(result == (int)strlen(input1));
    assert(strcmp(buffer, input1) == 0);
    printf("Test 1 passed");

    // Test 2: Buffer is NULL
    result = mini_scanf(NULL, sizeof(buffer));
    assert(result == -1);
    printf("Test 2 passed");

    // Test 3: size_buffer is zero
    result = mini_scanf(buffer, 0);
    assert(result == -1);
    printf("Test 3 passed");

    // Test 4: size_buffer is negative
    result = mini_scanf(buffer, -1);
    assert(result == -1);
    printf("Test 4 passed");

    // Test 5: Input larger than buffer size
    const char *input2 = "This is a very long input string that exceeds the buffer size.";
    write(STDIN_FILENO, input2, strlen(input2));
    result = mini_scanf(buffer, 10);
    assert(result == 10);
    buffer[10] = '\0'; // Null-terminate the string for comparison
    assert(strncmp(buffer, input2, 10) == 0);
    printf("Test 5 passed");
    printf("All tests passed!\n");
}

int main() {
    test_mini_calloc();
    test_mini_free();
    test_mini_scanf();
    test_mini_printf();
    test_mini_exit();
    return 0;
}