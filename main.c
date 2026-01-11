#include "header.h"                // Include header

char prompt[25] = "Minishell: ";   // Shell prompt
char input_string[25];             // Input buffer

int main(int argc, char** argv)    // Main function
{
    system("clear");               // Clear terminal screen
    scan_input(prompt, input_string); // Start shell
}
