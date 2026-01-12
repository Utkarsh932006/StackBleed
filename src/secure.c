/**
 * Stack Adjacency Leak (Mitigated)
 * * This version implements explicit bounds checking to prevent
 * the buffer over-read.
 */

#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char **argv)
{
    if (argc < 2) {
        printf("Usage: %s <number_of_reads> [optional_args...]\n", argv[0]);
        return 1;
    }

    int n_inputs = atoi(argv[1]);

    printf("--- Starting Memory Read Loop (SECURE) ---\n");

    for (int i = 0; i < n_inputs; i++)
    {
        // MITIGATION:
        // Explicitly check if the index we are about to access 
        // is within the bounds of the arguments provided.
        if ((i + 2) >= argc) {
            fprintf(stderr, "[!] STOP: Attempted to read out of bounds at index %d\n", i+2);
            break;
        }

        printf("[%d]: %s\n", i + 2, argv[i+2]);
    }

    return 0;
}
