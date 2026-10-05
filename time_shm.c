#include <stdio.h>
#include <stdlib.h>
#include <unisted.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

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

    // create a shared memory before fork() so both of the processes can see it
    // shm_open creates the shared memory and gives back a number (fd)
    shm_fd = shm_open("/pa2_time", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        printf("shm_open failed\n");
        return 1;
    }

    // ftruncate sets how many bytes it holds. room for one struct timeval
    if (ftruncate(shm_fd, sizeof(struct timeval)) == -1) {
        printf("ftruncate failed\n");
        return 1;
    }

    // mmap connects the shared memory to our 'start' pointer
    start = mmap(NULL, sizeof(struct timeval), PROT_READ | PROT_WRITE,
                 MAP_SHARED, shm_fd, 0);
    if (start == MAP_FAILED) {
        printf("mmap failed\n");
        return 1;
    }

}