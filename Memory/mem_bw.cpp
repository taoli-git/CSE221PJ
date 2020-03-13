#include <iostream>
//#include <mach/mach_time.h>

#define ITERATIONS 100
#define PAGE (1<<12) //4KB
#define ARRAY_SIZE (1<<20) //1MB
#define BYTES_PER_GB (1<<30)
#define NANOS_PER_SECF 1000000000.0
#define CACHE_SIZE (1<<22) //4MB

static inline  uint64_t rdtsc() //unsigned long long
{
    uint32_t lo,hi;

    __asm__ __volatile__
    (
    "rdtsc":"=a"(lo),"=d"(hi)
    );
    return (((uint64_t)hi)<<32|lo);
}


int foo[CACHE_SIZE], bar[CACHE_SIZE];
#define FILL_CACHE memcpy(foo, bar, CACHE_SIZE);


/* Get the timebase info */
//static mach_timebase_info_data_t info;

//static void __attribute__((constructor)) init_info() {
//    mach_timebase_info(&info);
//}

//double to_bw(size_t bytes, double secs);
//double monotonic_time();
//uint64_t wr(int *beg, int *end);
//uint64_t rd(int *beg, int *end);

/* GB/SEC */
int S = 0;

//double to_bw(uint64_t time){
//    //((1<<22)*1.0 / (1<<30) * 2.7e9 / (t))
//    int Bytes = (ARRAY_SIZE+PAGE)*4;
//    printf("Bytes: %d", Bytes);
//    double GB = Bytes/BYTES_PER_GB;
//    printf("GB: %d", GB);
//    uint64_t sec = time/2.7e9;
//    return GB/sec;
//}

///* sec */
//double monotonic_time() {
//    uint64_t time = mach_absolute_time();
//    double dtime = (double) time;
//    dtime *= (double) info.numer;
//    dtime /= (double) info.denom;
//    return dtime / NANOS_PER_SECF;
//}


uint64_t rd(int *beg, int *end){
    int sum = 0;
    uint64_t start;
    uint64_t ending;
    uint64_t total_time = 0;

    for(int i = 0; i< ITERATIONS; i++){
        int *p = beg;
        /*fill cache */
        FILL_CACHE;
        start = rdtsc();
        while(p< end){
#define OP(j) sum += p[j];
            OP(0)  OP(16)  OP(32)  OP(48) OP(64) OP(80)  OP(96)  OP(112) OP(128)  OP(144)  OP(160)  OP(176)  OP(192)  OP(208)   OP(224)  OP(240)  OP(256)  OP(272)  OP(288)  OP(304)   OP(320)  OP(336)   OP(352)  OP(368)  OP(384)  OP(400)  OP(416)  OP(432)   OP(448)  OP(464)  OP(480)  OP(496)
            p += 512;
        }
        ending = rdtsc();
        total_time += ending - start;
    }
    S += sum;
    return total_time/ITERATIONS;
}
#undef OP

uint64_t wr(int *beg, int *end){
    uint64_t start;
    uint64_t ending;
    uint64_t total_time = 0;

    for(int i = 0; i< ITERATIONS; i++){
        int *p = beg;
        /*fill cache */
        FILL_CACHE;
        start = rdtsc();
        while(p< end){
#define OP(j) p[j] = 1;
            OP(0)  OP(16)  OP(32)  OP(48) OP(64) OP(80)  OP(96)  OP(112) OP(128)  OP(144)  OP(160)  OP(176)  OP(192)  OP(208)   OP(224)  OP(240)  OP(256)  OP(272)  OP(288)  OP(304)   OP(320)  OP(336)   OP(352)  OP(368)  OP(384)  OP(400)  OP(416)  OP(432)   OP(448)  OP(464)  OP(480)  OP(496)
            p += 512;
        }
        ending = rdtsc();
        total_time += ending-start;
    }
    return total_time/ITERATIONS;
}
#undef OP

int main(){
    // Have PAGE_SIZE buffering so we don't have to do math for prefetching.
    //ARRAY_SIZE 4MB + PAGE_SIZE 4KB

    uint64_t write, read;
    int arr[ARRAY_SIZE+PAGE];


    //std::cout << 1;

    read = rd(arr,arr+ARRAY_SIZE+PAGE);
    write = wr(arr,arr+ARRAY_SIZE+PAGE);
    //std::cout << 2;
#define to_bw(t) ((1<<22)*1.0 / (1<<30) * 2.7e9 / (t))

    printf("Read bandwidth: %.5f (GB/s); Write bandwidth: %.5f (GB/s)\n", to_bw(read), to_bw(write));
    //printf("Read bandwidth: %.5f (GB/s); Write bandwidth: %.5f (GB/s)\n", read, write);
#undef to_bw
}


