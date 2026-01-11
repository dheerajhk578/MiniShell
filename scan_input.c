#include "header.h"
#include <stdio.h>
#include <string.h>

char* external_commands[155];      // External command list
int status;                        // Child process status
int in_pipeline = 0;               // Pipeline flag

void trim_trailing_spaces(char *str) // Remove trailing spaces
{
    int i = strlen(str) - 1;       // Start from last index
    while (i >= 0 && str[i] == ' ')// While space exists
    {
        str[i] = '\0';             // Remove space
        i--;                       // Move left
    }
}

void scan_input(char *prompt, char *input_string)
{
    signal(SIGINT,signal_handler); // Handle Ctrl+C
    signal(SIGTSTP,signal_handler);// Handle Ctrl+Z

    extract_external_commands(external_commands); // Load external commands

    while (true)                   // Infinite loop
    {
        printf("\033[1;36m");
printf("%s",prompt);
printf("\033[01;33m");
printf("$ ");
printf("\033[0m");

        input_string[0] = '\0';    // Clear input buffer
        scanf("%24[^\n]",input_string); // Read input
        getchar();                 // Clear newline

        trim_trailing_spaces(input_string); // FIX: remove trailing spaces

        if ( strncmp(input_string,"PS1=",4) == 0 ) // Change prompt
        {
            if ( input_string[4] != ' ' )
                strcpy(prompt,input_string+4); // Set new prompt
            else
                perror("PS1");      // Error
        }
        else
        {
            char* command = get_command(input_string); // Get command

            if ( command == NULL )  // If null command
                goto error_handling_command;

            int type = check_command_type(command); // Get type

            switch (type)
            {
                case BUILTIN:       // Builtin command
                    execute_internal_commands(input_string);
                    break;

                case EXTERNAL:      // External command
                {
                    if ( pipecheck(input_string) ) // If pipe present
                    {
                        pipe_operation(input_string); // Execute pipe
                    }
                    else
                    {
                        extern pid_t pid; // Child PID
                        pid = fork();     // Create process

                        if ( pid == 0 )   // Child
                        {
                            signal(SIGINT,SIG_DFL);  // Default Ctrl+C
                            signal(SIGTSTP,SIG_DFL); // Default Ctrl+Z
                            execute_external_commands(input_string,command); // Exec
                            exit(1);       // Exit child
                        }
                        else
                        {
                            waitpid(pid, &status, WUNTRACED); // Wait child
                            pid = 0;       // Reset pid
                        }
                    }
                    break;
                }

                case NO_COMMAND:    // Invalid command
                    if ( strlen(command) != 0 )
                        printf("%s: command not found\n",command);
                    break;
            }

        error_handling_command:
            free(command);          // Free memory
        }
    }
}
