//Time_pipe
//Group 6
//Student Name: Kaleb, Mathew Cano, Felix, Manuel Jaimes, Mathew Rodriguez
//PA2: Processes, Timing, and Interprocess Communication (IPC)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    // variables and argument check
    int fd[2]; // fd[0] = read end, fd[1] = write end
    struct timeval start; // start time
    struct timeval end; // end time
    int pid; // process id returned by fork()
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
        perror("pipe failed");
        return 1;
    }

    // create child process
    pid = fork();

    if (pid == -1) {
        perror("fork failed");
        close(fd[0]);
        close(fd[1]);
        return 1;
    }

    if (pid == 0) {
        
        // Child process
        close(fd[0]);

        // record starting time
        if (gettimeofday(&start, NULL) == -1) {
            perror("gettimeofday failed");
            close(fd[1]);
            _exit(1);
        }

        // send starting time to parent
        if (write(fd[1], &start, sizeof(start)) != sizeof(start)) {
            perror("write failed");
            close(fd[1]);
            _exit(1);
        }

        close(fd[1]); 

        // execute user command
        execvp(argv[1], &argv[1]);

        // only run if execvp() fails
        perror("execvp failed");
        _exit(127);
    }
    else {

        // Parent process
        close(fd[1]); 

        // wait for child process to finish
        if (waitpid(pid, NULL, 0) == -1) {
            perror("waitpid failed");
            close(fd[0]);
            return 1;
        }

        // read starting time
        ssize_t bytes_read = read(fd[0], &start, sizeof(start));
        close(fd[0]);

        if (bytes_read != sizeof(start)) {
            fprintf(stderr, "Failed to read starting time\n");
            return 1;
        }

        // record ending time
        if (gettimeofday(&end, NULL) == -1) {
            perror("gettimeofday failed");
            return 1;
        }

        // calculate elapsed time
        seconds_part = (double)(end.tv_sec - start.tv_sec);
        micro_part = (double)(end.tv_usec - start.tv_usec) / 1000000.0;
        elapsed = seconds_part + micro_part;

        printf("Elapsed time: %.6f seconds\n", elapsed);
    }

    return 0;
}
