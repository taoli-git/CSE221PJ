#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


const off_t blocksize = 4*1024; //4KB

static inline  uint64_t rdtsc() //unsigned long long
{
    uint32_t lo,hi;

    __asm__ __volatile__
    (
    "rdtsc":"=a"(lo),"=d"(hi)
    );
    return (((uint64_t)hi)<<32|lo);
}

int main(int argc, const char * argv[]){
    void* buffer = malloc(blocksize);
    const off_t filesize = atoll(argv[1])*1024*1024; //MB to B

    int fd = open(argv[2], O_RDONLY | O_SYNC);

    //warm up
    //start from end to beginning to avoid data prefetch
    lseek(fd, filesize - 1, SEEK_SET); //set offset of the last block
    size_t total = 0;

    while(true){
        //read by one block
        lseek(fd, -2*blocksize, SEEK_CUR);
        size_t bytes = read(fd,buffer,blocksize);
        if(bytes <= 0 || total >= filesize){
            break;
        }
        total += bytes;
    }

    close(fd);


    //start experiment
    uint64_t start;
    uint64_t end;
    uint64_t sum = 0;

    //10 iterations
    for(int i=0;i <10; i++){

        uint64_t total_time = 0;

        fd = open(argv[2], O_RDONLY | O_SYNC);
        total = 0;

        lseek(fd, filesize - 1, SEEK_SET); //reset offset of the last block
        while(true){
            lseek(fd, -2*blocksize, SEEK_CUR);
            //only timing the read operation
            start = rdtsc();
            size_t bytes = read(fd,buffer,blocksize);
            //printf("bytes: %zu\n",bytes);
            end = rdtsc();
            if(bytes <= 0 || total >= filesize){
                break;
            }
            total_time += end-start;
            total += bytes;
        }
        close(fd);
        sum += total_time;

    }

    free(buffer);
    double Ave_read = (double) sum * 1000000/(filesize*10/blocksize); //sec to μsec
    printf("Average read time for filesize %lld is %lf\n", filesize, Ave_read);
    return 0;

}
