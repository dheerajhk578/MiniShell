#include "header.h"
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

Stop signal_details[50];          // Array to store stopped jobs
int signal_details_index = 0;     // Index for stopped jobs array

void signal_handler(int signum)
{
    extern pid_t pid;             // Current running child pid

    if (signum == SIGINT)         // Ctrl + C
    {
        // Do nothing here
        // Prompt will be printed by scan_input() loop
    }
    else if (signum == SIGTSTP)   // Ctrl + Z
    {
        if (pid != 0)             // If a child process is running
        {
            extern char input_string[25];  // Current command
            signal_details[signal_details_index].spid = pid; // Store pid
            strcpy(signal_details[signal_details_index].name, input_string); // Store command
            signal_details_index++;         // Increment job index
        }
        // Do NOT print prompt here
    }
    else if (signum == SIGCHLD)   // Child process terminated
    {
        int status;
        pid_t terminated_pid = waitpid(-1, &status, WNOHANG); // Reap child

        if (terminated_pid > 0)
        {
            // Remove the terminated process from stopped jobs list
            for (int i = 0; i < signal_details_index; i++)
            {
                if (signal_details[i].spid == terminated_pid)
                {
                    for (int j = i; j < signal_details_index - 1; j++)
                        signal_details[j] = signal_details[j + 1];

                    signal_details_index--; // Decrease job count
                    break;
                }
            }
        }
    }
}
