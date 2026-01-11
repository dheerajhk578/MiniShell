#include "header.h"

char *builtins[] = {                // Builtin commands list
    "echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
    "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
    "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help",
    "fg","bg","jobs", NULL
};

int external_commands_count = 0;    // External command count
pid_t pid;                          // Global pid

int pipecheck(char* input_string)   // Check for pipe
{
    int i = 0;
    while (input_string[i])
    {
        if ( input_string[i] == '|' )
            return 1;               // Pipe found
        i++;
    }
    return 0;                       // No pipe
}

void pipe_operation(char *input_string)
{
    in_pipeline = 1;                // Mark pipeline start

    char input_copy[1024];          // Copy buffer
    strncpy(input_copy, input_string, sizeof(input_copy) - 1);
    input_copy[sizeof(input_copy) - 1] = '\0';

    char *argv[100];                // Argument array
    int argc = 0;                   // Arg count

    char *token = strtok(input_copy, " "); // Tokenize
    while (token != NULL)
    {
        argv[argc++] = token;       // Store token
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL;              // Null terminate

    int command_index[50];          // Command index array
    int total_pipe_count = 0;       // Pipe count
    int cmd_count = 0;              // Command count

    command_index[cmd_count++] = 0; // First command

    for (int i = 0; i < argc; i++)  // Find pipes
    {
        if (strcmp(argv[i], "|") == 0)
        {
            argv[i] = NULL;         // Split command
            command_index[cmd_count++] = i + 1;
            total_pipe_count++;
        }
    }

    int pipes[total_pipe_count][2]; // Pipe fd array

    for (int i = 0; i < total_pipe_count; i++)
        pipe(pipes[i]);             // Create pipes

    pid_t pids[50];                 // Child pid array

    for (int i = 0; i <= total_pipe_count; i++)
    {
        pid_t pid_local = fork();   // Fork
        pids[i] = pid_local;        // Store pid

        if (pid_local == 0)         // Child
        {
            signal(SIGINT, SIG_DFL);// Default Ctrl+C
            signal(SIGTSTP, SIG_DFL);// Default Ctrl+Z

            if (i > 0)
                dup2(pipes[i - 1][0], STDIN_FILENO); // Input redirection

            if (i < total_pipe_count)
                dup2(pipes[i][1], STDOUT_FILENO);    // Output redirection

            for (int j = 0; j < total_pipe_count; j++)
            {
                close(pipes[j][0]); // Close read end
                close(pipes[j][1]); // Close write end
            }

            execvp(argv[command_index[i]], &argv[command_index[i]]); // Execute
            perror("execvp");        // Error
            exit(1);                // Exit child
        }
    }

    for (int i = 0; i < total_pipe_count; i++)
    {
        close(pipes[i][0]);         // Close read
        close(pipes[i][1]);         // Close write
    }

    for (int i = 0; i <= total_pipe_count; i++)
        waitpid(pids[i], NULL, 0);  // Wait all children

    in_pipeline = 0;                // Mark pipeline end
}

void extract_external_commands(char **external_commands)
{
    int fd = open("external_commands.txt", O_RDONLY); // Open file
    if (fd < 0)
    {
        perror("open");             // Error
        return;
    }

    char ch;                        // Char buffer
    char temp[100];                 // Temp buffer
    int i = 0;                      // Index

    while (read(fd, &ch, 1) > 0)    // Read file
    {
        if (ch != '\n')
            temp[i++] = ch;         // Store char
        else
        {
            temp[i] = '\0';         // Null terminate
            external_commands[external_commands_count] = malloc(strlen(temp) + 1); // Allocate
            strcpy(external_commands[external_commands_count], temp); // Copy
            external_commands_count++; // Increment count
            i = 0;                  // Reset index
        }
    }
    close(fd);                      // Close file
}

char *get_command(char *input_string)
{
    char tmp[100];                  // Temp buffer
    int i = 0;                      // Index

    if (input_string[0] == ' ')     // Leading space
        return NULL;

    while (input_string[i] != ' ' && input_string[i] != '\0') // Copy till space
    {
        tmp[i] = input_string[i];   // Copy char
        i++;
    }
    tmp[i] = '\0';                  // Null terminate

    char *cmd = malloc(strlen(tmp) + 1); // Allocate
    strcpy(cmd, tmp);               // Copy
    return cmd;                     // Return command
}

int check_command_type(char *command)
{
    int i = 0;
    while (builtins[i] != NULL)     // Check builtins
    {
        if (strcmp(command, builtins[i]) == 0)
            return BUILTIN;         // Builtin found
        i++;
    }

    extern char* external_commands[155]; // External list
    for (int i = 0; i < external_commands_count; i++)
    {
        if (strcmp(external_commands[i], command) == 0)
            return EXTERNAL;        // External found
    }

    return NO_COMMAND;              // Not found
}

void execute_external_commands(char *input_string,char* command)
{
    char input_copy[1024];          // Copy buffer
    strcpy(input_copy, input_string);

    char *argv[20];                 // Argument array
    int i = 0;                      // Index

    argv[i++] = command;            // First arg

    char *args = input_copy + strlen(command); // Get args
    while (*args == ' ') args++;    // Skip spaces

    char *token = strtok(args, " "); // Tokenize
    while (token != NULL)
    {
        argv[i++] = token;          // Add arg
        token = strtok(NULL, " ");  // Next token
    }

    argv[i] = NULL;                 // Null terminate

    execvp(argv[0], argv);          // Execute
    perror("execvp");               // Error
    exit(1);                        // Exit
}

void execute_internal_commands(char *input_string)
{
    if (strncmp(input_string,"exit",4) == 0) // Exit
        exit(0);

    else if (strncmp(input_string,"pwd",3) == 0) // pwd
    {
        char buf[500];
        getcwd(buf,sizeof(buf));     // Get cwd
        printf("%s\n",buf);          // Print
    }
    else if (strncmp(input_string,"cd",2) == 0) // cd
    {
        char *path = input_string + 3;
        chdir(path);                 // Change dir
    }
    else if (strcmp(input_string,"jobs") == 0) // jobs
    {
        extern int signal_details_index;
        extern Stop signal_details[50];

        for (int i = 0; i < signal_details_index; i++)
            printf("[%d] %s\n",i+1,signal_details[i].name); // Print jobs
    }
    else if (strcmp(input_string,"fg") == 0) // fg
    {
        extern int signal_details_index;
        extern Stop signal_details[50];

        if (signal_details_index == 0)
        {
            printf("fg: no current job\n");
            return;
        }

        int idx = signal_details_index - 1;
        kill(signal_details[idx].spid,SIGCONT); // Continue
        waitpid(signal_details[idx].spid,NULL,WUNTRACED); // Wait
        signal_details_index--;      // Remove job
    }
    else if (strcmp(input_string,"bg") == 0) // bg
    {
        extern int signal_details_index;
        extern Stop signal_details[50];

        if (signal_details_index == 0)
        {
            printf("bg: no current job\n");
            return;
        }

        int idx = signal_details_index - 1;
        kill(signal_details[idx].spid,SIGCONT); // Continue bg
    }
}
