#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Authors:
   Abhinav Sasikumar
   Farhan Syed

   2023-09-18

   Compilation: gcc partOne.c -o partOne.out
   Execution:   ./partOne.out

*/

int fd[2];

int main(int argc, char const *argv[])
{

    pipe(fd);
    pid_t pid = fork();

    if (pid == 0)
    {
        close(fd[0]);                       // Close read end
        dup2(fd[1], STDOUT_FILENO);         // Send execution output to pipe
        execlp("/bin/ls", "ls", "/", NULL); // Execute ls command
    }
    else if (pid > 0)
    {
        pid = wait(NULL);                   // Wait for child to finish

        close(fd[1]);                        // Close write end
        dup2(fd[0], STDIN_FILENO);           // Send info from pipe as input to next execution
        execlp("/bin/wc", "wc", "-l", NULL); // Execute wordcount command
    }
    return 0;
}
