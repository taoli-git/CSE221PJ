#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


const off_t blocksize = 4*1024; //4KB
const char* files[] = {"file1", "file2","file3", "file4","file5", "file6","file7", "file8","file9", "file10"};
off_t filesize;


static inline  uint64_t rdtsc() //unsigned long long
{
    uint32_t lo,hi;

    __asm__ __volatile__
    (
    "rdtsc":"=a"(lo),"=d"(hi)
    );
    return (((uint64_t)hi)<<32|lo);
}

void readFile(int n){
    void* buffer = malloc(blocksize);
    int fd = open(files[n], O_RDONLY);



    uint64_t start;
    uint64_t end;

    uint64_t total_time = 0;
    size_t total_bytes = 0;


    while(true) {
        //only timing the read operation
        start = rdtsc();
        size_t bytes = read(fd, buffer, blocksize);
        end = rdtsc();



        if (bytes <= 0 || total_bytes >= filesize) {
            break;
        }
        total_bytes += bytes;
        total_time += end - start;
    }
    close(fd);
    double time_pb = (double) total_time/(filesize/blocksize);
    printf("process %d takes %lf to read a block\n", n, time_pb);
}

int main(int argc, const char * argv[]) {
    pid_t procs[10];
    int num = atoll(argv[1]); //how many processes to create;
    filesize = atoll(argv[2])*1024*1024;
    printf("filesize: %d\n", filesize);


    for(int i = 0; i<num; i++){
        if((procs[i] = fork())== 0){
            while(true){
                readFile(i);
            }
            }
        }
    int pid;

    while (pid = waitpid(-1, NULL, 0)) {
        if (errno == ECHILD) {
            break;
        }
    }
    return 0;
    }

