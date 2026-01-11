#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>      // printf, scanf
#include <stdbool.h>    // bool type
#include <string.h>     // string functions
#include <stdlib.h>     // malloc, free, exit
#include <stdio_ext.h>  // __fpurge
#include <sys/types.h>  // pid_t
#include <sys/stat.h>   // file status
#include <fcntl.h>      // open
#include <unistd.h>     // fork, exec, chdir
#include <signal.h>     // signal handling
#include <sys/wait.h>   // wait, waitpid

#define BUILTIN     1
#define EXTERNAL    2 
#define NO_COMMAND  3

/* ===== ANSI COLOR MACROS ===== */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* ===== JOB STRUCT (YOUR ORIGINAL) ===== */
typedef struct {
    int spid;
    char name[15];
} Stop;

extern int in_pipeline;                 // FIX: pipeline flag

void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);
void copy_change(char *prompt, char *input_string);
int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void execute_external_commands(char *input_string,char* command);
void signal_handler(int sig_num);
void extract_external_commands(char **external_commands); 
int pipecheck(char* input_string );
void pipe_operation( char* input_string );
void trim_trailing_spaces(char *str);   // FIX: for ls␣ issue

#endif
