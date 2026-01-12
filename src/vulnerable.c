/**
 * Stack Adjacency Leak PoC (Vulnerable)
 * * Demonstration of how a logic error (buffer over-read) allows
 * reading past the Argument Vector (argv) into the Environment 
 * Variables (envp) because they are contiguous on the stack.
 * * NOTE: Standard tools like AddressSanitizer (ASan) often miss 
 * this because the memory read is within valid stack space 
 * allocated by the OS for the process.
 */

#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char **argv)
{
    // Ensure we have at least one argument for the count
    if (argc < 2) {
        printf("Usage: %s <number_of_reads> [optional_args...]\n", argv[0]);
        return 1;
    }

    // Convert first argument to integer (User Input)
    int n_inputs = atoi(argv[1]);

    printf("--- Starting Memory Read Loop ---\n");
    printf("Target: Read %d slots starting from argv[2]\n", n_inputs);
    printf("-------------------------------------\n");

    // VULNERABILITY:
    // The loop relies on 'n_inputs' (user controlled) effectively trusting
    // the user not to ask for more items than exist.
    // There is NO check against 'argc'.
    for (int i = 0; i < n_inputs; i++)
    {
        // On Linux, if we go past argv[argc], we hit NULL, 
        // and then immediately hit envp[0].
        printf("[%d]: %s\n", i + 2, argv[i+2]);
    }

    return 0;
}
