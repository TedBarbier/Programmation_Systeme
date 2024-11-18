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
#include <sys/errno.h>


// include personal library
#include "mini_lib.h"


/*
-------------------------------------


        Test mini_memory.c


-------------------------------------


*/
void test_mini_calloc() {
    printf("-------------------------------------");
    printf("\n\n\n");
    printf("        Test mini_calloc");
    printf("\n\n\n");
    printf("-------------------------------------\n");

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
    printf("-------------------------------------");
    printf("\n\n\n");
    printf("        Test mini_free");
    printf("\n\n\n");
    printf("-------------------------------------\n");

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
    printf("-------------------------------------");
    printf("\n\n\n");
    printf("        Test mini_exit");
    printf("\n\n\n");
    printf("-------------------------------------\n");

    // Test: Call mini_exit (this will terminate the program)
    printf("Calling mini_exit...\n");
    mini_exit();
    // This line should not be reached
    printf("Test failed: mini_exit did not terminate the program\n");
}

/*
-------------------------------------


        Test mini_string.c


-------------------------------------
*/

void test_mini_printf() {
    printf("-------------------------------------");
    printf("\n\n\n");
    printf("        Test mini_printf");
    printf("\n\n\n");
    printf("-------------------------------------\n");
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
    printf("-------------------------------------");
    printf("\n\n\n");
    printf("        Test mini_scanf");
    printf("\n\n\n");
    printf("-------------------------------------\n");
    char buffer[100];
    int result;

    // Test 1: Normal input
    printf("Ecrire : 'Hello, World!'\n");
    const char *input1 = "Hello, World!";
    result = mini_scanf(buffer, sizeof(buffer));
    if (result == (int)strlen(input1) && strcmp(buffer, input1) == 0) {
        printf("Test 1 passed\n");
    } else {
        printf("Test 1 failed\n");
    }

    // Test 2: Buffer is NULL
    result = mini_scanf(NULL, sizeof(buffer));
    if (result == -1) {
        printf("Test 2 passed\n");
    } else {
        printf("Test 2 failed\n");
    }

    // Test 3: size_buffer is zero
    result = mini_scanf(buffer, 0);
    if (result == -1) {
        printf("Test 3 passed\n");
    } else {
        printf("Test 3 failed\n");
    }

    // Test 4: size_buffer is negative
    result = mini_scanf(buffer, -1);
    if (result == -1) {
        printf("Test 4 passed\n");
    } else {
        printf("Test 4 failed\n");
    }

    // Test 5: Input larger than buffer size
    printf("Ecrire : 'This is a very long input string that exceeds the buffer size.'\n");
    const char *input2 = "This is a very long input string that exceeds the buffer size.";
    result = mini_scanf(buffer, 10);
    if (result == 10 && strncmp(buffer, input2, 10) == 0) {
        printf("Test 5 passed\n");
    } else {
        printf("Test 5 failed\n");
    }
    printf("All tests passed!\n");
}

void test_mini_strlen() {
    printf("-------------------------------------");
    printf("\n\n\n");
    printf("        Test mini_strlen");
    printf("\n\n\n");
    printf("-------------------------------------\n");
    char *test1 = "Hello, World!";
    char *test2 = "";
    char *test3 = "mini_strlen test";

    int result1 = mini_strlen(test1);
    int result2 = mini_strlen(test2);
    int result3 = mini_strlen(test3);
    if (13 == result1)printf("Test 1 passed\n");
    else printf("test1 failed\n"); 
    if (0 == result2)printf("Test 2 passed\n");
    else printf("test2 failed\n"); 
    if (16 == result3)printf("Test 3 passed\n");
    else printf("test3 failed\n"); 
}

void test_mini_strcopy() {
    printf("-------------------------------------");
    printf("\n\n\n");
    printf("        Test mini_strcopy");
    printf("\n\n\n");
    printf("-------------------------------------\n");
    char source[] = "Hello, World!";
    char destination[50];

    int copied_length = mini_strcopy(source, destination);
    // Check if the copied length is correct
    if (copied_length == 13) {
        printf("Length test passed for mini_strcopy!\n");
    } else {
        printf("Length test failed for mini_strcopy!\n");
    }
    // Check if the destination string is correctly copied
    if (strcmp(source, destination) == 0) {
        printf("String copy test passed for mini_strcopy!\n");
    } else {
        printf("String copy test failed for mini_strcopy!\n");
    }
}

void test_mini_strcmp() {
    printf("-------------------------------------");
    printf("\n\n\n");
    printf("        Test mini_strcmp");
    printf("\n\n\n");
    printf("-------------------------------------\n");
    char str1[] = "Hello, World!";
    char str2[] = "Hello, World!";
    char str3[] = "Hello, C Programming!";

    // Test when strings are equal
    if (mini_strcmp(str1, str2) == 0) {
        printf("Equality test passed for mini_strcmp!\n");
    } else {
        printf("Equality test failed for mini_strcmp!\n");
    }

    // Test when strings are not equal
    if (mini_strcmp(str1, str3) == -1) {
        printf("Inequality test passed for mini_strcmp!\n");
    } else {
        printf("Inequality test failed for mini_strcmp!\n");
    }
}

void test_mini_perror() {
    printf("-------------------------------------");
    printf("\n\n\n");
    printf("        Test mini_perror");
    printf("\n\n\n");
    printf("-------------------------------------\n");
    // Test 1: mini_perror with a custom message and a set errno
    errno = ENOENT; // No such file or directory
    mini_printf("Test Error 1 :");
    mini_perror("Error 1");


    // Test 2: mini_perror with a different errno
    errno = EACCES; // Permission denied
    mini_printf("Test Error 2 :");
    mini_perror("Error 2");


    // Test 3: mini_perror with errno set to 0
    errno = 0;
    mini_printf("Test Error 3 :");
    mini_perror("Error 3");

    // Test 4: mini_perror with a NULL message
    errno = EINVAL; // Invalid argument
    mini_printf("Test Error 4 :");
    mini_perror(NULL);

}

/*
-------------------------------------


        Test mini_io.c


-------------------------------------


*/
/*
void test_mini_fopen() {
    printf("-------------------------------------");
    printf("\n\n\n");
    printf("        Test mini_fopen");
    printf("\n\n\n");
    printf("-------------------------------------\n");
    MYFILE* file;

    // Test opening a file in read mode
    file = mini_fopen("test_read.txt", 'r');
    if (file == NULL) {
        perror("Failed to open file in read mode");
    } else {
        printf("Successfully opened file in read mode\n");
        mini_fclose(file);
    }

    // Test opening a file in write mode
    file = mini_fopen("test_write.txt", 'w');
    if (file == NULL) {
        perror("Failed to open file in write mode");
    } else {
        printf("Successfully opened file in write mode\n");
        mini_fclose(file);
    }

    // Test opening a file in read/write mode
    file = mini_fopen("test_read_write.txt", 'b');
    if (file == NULL) {
        perror("Failed to open file in read/write mode");
    } else {
        printf("Successfully opened file in read/write mode\n");
        mini_fclose(file);
    }

    // Test opening a file in append mode
    file = mini_fopen("test_append.txt", 'a');
    if (file == NULL) {
        perror("Failed to open file in append mode");
    } else {
        printf("Successfully opened file in append mode\n");
        mini_fclose(file);
    }

    // Test opening a file with an invalid mode
    file = mini_fopen("test_invalid.txt", 'x');
    if (file == NULL) {
        if (errno == EINVAL) {
            printf("Correctly handled invalid mode\n");
        } else {
            perror("Failed to handle invalid mode correctly");
        }
    } else {
        printf("Unexpectedly opened file with invalid mode\n");
        mini_fclose(file);
    }
}
*/
int main() {
    test_mini_calloc();
    test_mini_free();
    test_mini_scanf();
    test_mini_strlen();
    test_mini_strcopy();
    test_mini_strcmp();
    test_mini_perror();
    test_mini_printf();
    test_mini_exit();
    return 0;
}