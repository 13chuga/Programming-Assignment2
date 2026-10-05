#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    // variables and argument check
    int fd[2]; // fd[] = read end, fd[1] = write end
    struct timeval start; // start time
    struct timeval end; // end time
    int pid; // process id return by fork()
    double seconds_part; // whole seconds between start and end
    double micro_part; // leftover microseconds between start and end
    double elapsed; // final answer in seconds

    if (argc < 2) {
        printf("Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    // creates the pipe
    // this must happen before fork() so both processes get the pipe
    // pipe() fills in fd[0] (read end) and fd[1] (write end)
    if (pipe(fd) == -1) {
        printf("pipe failed\n");
        return 1;
    }
}