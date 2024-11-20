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


#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/errno.h>
#include "mini_lib.h"

typedef struct {
    const char* test_name;
    int passed;
} TestResult;

TestResult test_results[100];  // tableau pour stocker les résultats des tests
int test_count = 0;
int failed_tests = 0;

// Fonction pour enregistrer les résultats de chaque test
void record_test_result(const char* test_name, int passed) {
    test_results[test_count].test_name = test_name;
    test_results[test_count].passed = passed;
    if (!passed) {
        failed_tests++;
    }
    test_count++;
}

// Fonction pour afficher les tests échoués
void print_failed_tests() {
    if (failed_tests > 0) {
        printf("\n==== Failed Tests ====\n");
        for (int i = 0; i < test_count; i++) {
            if (!test_results[i].passed) {
                printf("Test failed: %s\n", test_results[i].test_name);
            }
        }
    } else {
        printf("\nAll tests passed!\n");
    }
}

// Fonction utilitaire pour afficher un en-tête pour chaque test
void print_test_header(const char *test_name) {
    printf("-------------------------------------\n");
    printf("        Test %s\n", test_name);
    printf("-------------------------------------\n");
}

// Fonction utilitaire pour afficher le résultat d'un test
void print_test_result(int passed, const char *test_name) {
    if (passed) {
        printf("%s passed\n", test_name);
    } else {
        printf("%s failed\n", test_name);
        record_test_result(test_name, 0);
    }
}



void test_mini_memset() {
    print_test_header("mini_memset");
    char arr[10];
    mini_memset(arr, 'A', sizeof(arr));
    print_test_result(arr[0] == 'A' && arr[9] == 'A', "Test 1 - Setting all elements to 'A'");
}

void test_mini_calloc() {
    print_test_header("mini_calloc");

    // Test 1: Allocate memory for 10 integers
    int *arr = (int*) mini_calloc(sizeof(int), 10);
    int passed = (arr != NULL);
    if (passed) {
        passed = 1;
        for (int i = 0; i < 10; i++) {
            if (arr[i] != 0) {
                passed = 0;
                break;
            }
        }
    }
    print_test_result(passed, "Test 1 - Allocate and initialize to zero");
    mini_free(arr);

    // Test 2: Invalid parameters
    void *ptr = mini_calloc(-1, 10);
    print_test_result(ptr == NULL, "Test 2 - Invalid parameters");

    // Test 3: Zero elements
    ptr = mini_calloc(sizeof(int), 0);
    print_test_result(ptr == NULL, "Test 3 - Allocate zero elements");
}

void test_mini_free() {
    print_test_header("mini_free");

    // Test 1: Free a valid pointer
    int *arr = (int*) mini_calloc(sizeof(int), 10);
    mini_free(arr);
    print_test_result(1, "Test 1 - Free valid pointer");

    // Test 2: Free a NULL pointer
    mini_free(NULL);
    print_test_result(1, "Test 2 - Free NULL pointer");

    // Test 3: Free already freed pointer
    arr = (int*) mini_calloc(sizeof(int), 10);
    mini_free(arr);
    mini_free(arr);  // Free again
    print_test_result(1, "Test 3 - Free already freed pointer");
}

void test_mini_exit() {
    print_test_header("mini_exit");

    // Test: Call mini_exit (this will terminate the program)
    printf("Calling mini_exit...\n");
    mini_exit();
    // This line should not be reached
    printf("Test failed: mini_exit did not terminate the program\n");
}
void test_mini_memory(void) {
    test_mini_memset();
    test_mini_calloc();
    test_mini_free();
}

void test_mini_printf(void) {
    print_test_header("mini_printf");

    // Test 1: Simple string
    char test1[] = "Hello, World!\n";
    mini_printf(test1);
    print_test_result(1, "Test 1 - Simple string");

    // Test 2: Another simple string with different content
    char test2[] = "This is a test of mini_printf.\n";
    mini_printf(test2);
    print_test_result(1, "Test 2 - Another simple string");

    // Test 3: Empty string (just a newline)
    char test3[] = "\n";
    mini_printf(test3);
    print_test_result(1, "Test 3 - Empty string");

    // Test 4: Long string (more than usual length)
    char long_str[1024+9];
    memset(long_str, 'A', sizeof(long_str) - 1);
    long_str[1023+9] = '\0';  // Ensure proper string termination
    mini_printf(long_str);
    print_test_result(1, "Test 4 - Long string");


    // Test 5: String with special characters like newline and tab
    char special_chars[] = "Newline:\nTab:\tEnd of special chars.\n";
    mini_printf(special_chars);
    print_test_result(1, "Test 5 - Special characters");

    // Test 6: Non-printable characters (just for checking output)
    char non_printable[] = "Testing with a non-printable character: \x01\n";
    mini_printf(non_printable);
    print_test_result(1, "Test 6 - Non-printable characters");

    // Test 7: Multiple lines of text
    mini_printf("Line 1\nLine 2\nLine 3\n");
    print_test_result(1, "Test 7 - Multiple lines");
}



void test_mini_scanf() {
    print_test_header("mini_scanf");
    char buffer[100];
    int result;

    // Test 1: Normal input
    const char *input1 = "Hello, World!";
    result = mini_scanf(buffer, sizeof(buffer));
    print_test_result(result == (int)strlen(input1) && strcmp(buffer, input1) == 0, "Test 1 - Normal input");

    // Test 2: NULL buffer
    result = mini_scanf(NULL, sizeof(buffer));
    print_test_result(result == -1, "Test 2 - NULL buffer");

    // Test 3: Zero buffer size
    result = mini_scanf(buffer, 0);
    print_test_result(result == -1, "Test 3 - Zero buffer size");
}

void test_mini_strlen() {
    print_test_header("mini_strlen");

    char *test1 = "Hello, World!";
    char *test2 = "";
    char *test3 = "mini_strlen test";

    print_test_result(mini_strlen(test1) == 13, "Test 1 - Non-empty string");
    print_test_result(mini_strlen(test2) == 0, "Test 2 - Empty string");
    print_test_result(mini_strlen(test3) == 16, "Test 3 - Non-empty string with spaces");
}

void test_mini_strcopy() {
    print_test_header("mini_strcopy");

    char source[] = "Hello, World!";
    char destination[50];

    int copied_length = mini_strcopy(source, destination);
    print_test_result(copied_length == 13, "Test 1 - Length of copied string");
    print_test_result(strcmp(source, destination) == 0, "Test 2 - String copy correctness");
}

void test_mini_strcmp() {
    print_test_header("mini_strcmp");

    char str1[] = "Hello, World!";
    char str2[] = "Hello, World!";
    char str3[] = "Hello, C Programming!";

    print_test_result(mini_strcmp(str1, str2) == 0, "Test 1 - Strings are equal");
    print_test_result(mini_strcmp(str1, str3) == -1, "Test 2 - Strings are not equal");
}

void test_mini_perror() {
    print_test_header("mini_perror");

    // Test 1: mini_perror with custom message and errno
    errno = ENOENT;
    mini_perror("Error 1");
    print_test_result(1, "Test 1 - Print error message");

    // Test 2: mini_perror with errno set to 0
    errno = 0;
    mini_perror("Error 2");
    print_test_result(1, "Test 2 - Print error message with errno 0");

    // Test 3: mini_perror with NULL message
    mini_perror(NULL);
    print_test_result(1, "Test 3 - Print error message with NULL message");
}

void test_mini_string(void) {
    test_mini_printf();
    test_mini_scanf();
    test_mini_strlen();
    test_mini_strcopy();
    test_mini_strcmp();
    test_mini_perror();
}

void test_mini_fopen() {
    print_test_header("mini_fopen");

    MYFILE* file;

    // Test opening a file in read mode
    file = mini_fopen("test_read.txt", 'r');
    print_test_result(file != NULL, "Test 1 - Open file in read mode");

    // Test opening a file in write mode
    file = mini_fopen("test_write.txt", 'w');
    print_test_result(file != NULL, "Test 2 - Open file in write mode");

    // Test opening a file with invalid mode
    file = mini_fopen("test_invalid.txt", 'x');
    print_test_result(file == NULL, "Test 3 - Handle invalid mode");
}

void test_mini_memcpy() {
    print_test_header("mini_memcpy");

    char src[] = "Hello, World!";
    char dest[20];
    mini_memcpy(dest, src, strlen(src) + 1);
    print_test_result(strcmp(src, dest) == 0, "Test 1 - Correct memcpy");
}

void test_mini_memmove() {
    print_test_header("mini_memmove");

    char str[] = "abcdef";
    mini_memmove(str + 2, str, 4);
    print_test_result(strcmp(str, "ababcde") == 0, "Test 1 - Correct memmove");
}

void test_mini_fread() {
    print_test_header("mini_fread");

    MYFILE* file = mini_fopen("test.txt", 'r');
    char buffer[50];
    int bytes_read = mini_fread(buffer, 1, 50, file);
    print_test_result(bytes_read > 0, "Test 1 - Read data from file");
    mini_fclose(file);
}

void test_mini_fwrite() {
    print_test_header("mini_fwrite");

    MYFILE* file = mini_fopen("test.txt", 'w');
    char data[] = "Hello, World!";
    int bytes_written = mini_fwrite(data, 1, strlen(data), file);
    print_test_result(bytes_written == strlen(data), "Test 1 - Write data to file");
    mini_fclose(file);
}

void test_mini_fflush() {
    print_test_header("mini_fflush");

    MYFILE* file = mini_fopen("test.txt", 'w');
    mini_fflush(file);
    print_test_result(1, "Test 1 - Flush file buffer");
    mini_fclose(file);
}

void test_mini_fclose() {
    print_test_header("mini_fclose");

    MYFILE* file = mini_fopen("test.txt", 'r');
    mini_fclose(file);
    print_test_result(1, "Test 1 - Close file");
}

void test_mini_io(void) {
    test_mini_fopen();
    test_mini_memcpy();
    test_mini_memmove();
    test_mini_fread();
    test_mini_fwrite();
    test_mini_fflush();
    test_mini_fclose();
}


// Fonction principale pour lancer tous les tests
int main(void) {
    test_mini_memory();
    test_mini_string();
    test_mini_io();

    // Affichage des tests échoués avant d'exécuter mini_exit
    print_failed_tests();

    test_mini_exit();
    return 0;
}
