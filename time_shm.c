//Time_sharedmemory
//Group 6
//Student Name: Kaleb, Mathew Cano, Felix, Manuel Jaimes, Mathew Rodriguez
//PA2: Processes, Timing, and Interprocess Communication (IPC)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    // variables and argument check
    struct timeval *start;
    struct timeval end;
    int shm_fd;
    int pid;
    double elapsed;

    if (argc < 2) {
        printf("Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    // create a shared memory before fork() so both processes can see it
    // shm_open creates the shared memory and gives back a number (fd)
    shm_fd = shm_open("/pa2_time", O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1) {
        printf("shm_open failed\n");
        return 1;
    }

    // ftruncate sets how many bytes it holds. room for one struct timeval
    if (ftruncate(shm_fd, sizeof(struct timeval)) == -1) {
        printf("ftruncate failed\n");
        close(shm_fd);
        shm_unlink("/pa2_time");
        return 1;
    }

    // mmap connects the shared memory to our 'start' pointer
    start = mmap(NULL, sizeof(struct timeval),
                 PROT_READ | PROT_WRITE,
                 MAP_SHARED, shm_fd, 0);

    if (start == MAP_FAILED) {
        printf("mmap failed\n");
        close(shm_fd);
        shm_unlink("/pa2_time");
        return 1;
    }

    // create child process
    pid = fork();

    if (pid == -1) {
        printf("fork failed\n");

        munmap(start, sizeof(struct timeval));
        close(shm_fd);
        shm_unlink("/pa2_time");

        return 1;
    }

    if (pid == 0) {
        // Child process
        // record the starting time
        if (gettimeofday(start, NULL) == -1) {
            printf("gettimeofday failed\n");
            _exit(1);
        }

        // execute command
        execvp(argv[1], &argv[1]);

        // only run if execvp() fails
        perror("execvp failed");
        _exit(127);
    }
    else {
        // Parent process
        // wait for child to finish
        if (waitpid(pid, NULL, 0) == -1) {
            perror("waitpid failed");

            munmap(start, sizeof(struct timeval));
            close(shm_fd);
            shm_unlink("/pa2_time");

            return 1;
        }

        // record ending time
        if (gettimeofday(&end, NULL) == -1) {
            perror("gettimeofday failed");

            munmap(start, sizeof(struct timeval));
            close(shm_fd);
            shm_unlink("/pa2_time");

            return 1;
        }

        // calculate elapsed time
        elapsed = (double)(end.tv_sec - start->tv_sec)
                + (double)(end.tv_usec - start->tv_usec) / 1000000.0;

        printf("Elapsed time: %.6f seconds\n", elapsed);

        // clean up memory
        munmap(start, sizeof(struct timeval));
        close(shm_fd);
        shm_unlink("/pa2_time");
    }

    return 0;
}