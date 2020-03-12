#include <iostream>
#include <mach/mach_time.h>

#define PAGE (4*1024) //4KB
#define ARRAY_SIZE (1024*1024*4) //4MB
#define BYTES_PER_GB (1024*1024*1024LL)
#define TIMES 2
#define NANOS_PER_SECF 1000000000.0
#define CACHE_SIZE (1024*1024*4) //4MB


int foo[CACHE_SIZE], bar[CACHE_SIZE];
#define FILL_CACHE memcpy(foo, bar, CACHE_SIZE);


/* Get the timebase info */
static mach_timebase_info_data_t info;

static void __attribute__((constructor)) init_info() {
    mach_timebase_info(&info);
}

double to_bw(size_t bytes, double secs);
double monotonic_time();
double wt(int *beg, int *end);
double rd(int *beg, int *end);

/* GB/SEC */
double to_bw(size_t bytes, double secs){
    double Bytes = (double)bytes;
    double GB = Bytes/BYTES_PER_GB;
    return GB/secs;
}

/* sec */
double monotonic_time() {
    uint64_t time = mach_absolute_time();
    double dtime = (double) time;
    dtime *= (double) info.numer;
    dtime /= (double) info.denom;
    return dtime / NANOS_PER_SECF;
}


double rd(int *beg, int *end){
    double time_start = 0;
    double time_end = 0;
    double total_time = 0;


    for(int i = 0; i< TIMES; i++){
        int sum = 0;
        int *p = beg;
        /*fill cache */
        FILL_CACHE;
        time_start = monotonic_time();
        while(p< end){
            sum += p[i];
            p++;
#define OP(j) sum += p[j];
            OP(0)  OP(16)  OP(32)  OP(48) OP(64) OP(80)  OP(96)  OP(112) OP(128)  OP(144)  OP(160)  OP(176)  OP(192)  OP(208)   OP(224)  OP(240)  OP(256)  OP(272)  OP(288)  OP(304)   OP(320)  OP(336)   OP(352)  OP(368)  OP(384)  OP(400)  OP(416)  OP(432)   OP(448)  OP(464)  OP(480)  OP(496)
            p += 512;

        }
        time_end = monotonic_time();
        total_time += time_start - time_end;
    }
    return to_bw(TIMES * ARRAY_SIZE, total_time);
}

//#undef OP;

//double wr(int *beg, int *end){
//    double time_start = 0;
//    double time_end = 0;
//    double total_time = 0;
//
//    for(int i = 0; i< TIMES; i++){
//        int *p = beg;
//        /*fill cache */
//        FILL_CACHE;
//        time_start = monotonic_time();
//        while(p< end){
//#define OP(j) p[j] = 1;
//            OP(0)  OP(16)  OP(32)  OP(48) OP(64) OP(80)  OP(96)  OP(112) OP(128)  OP(144)  OP(160)  OP(176)  OP(192)  OP(208)   OP(224)  OP(240)  OP(256)  OP(272)  OP(288)  OP(304)   OP(320)  OP(336)   OP(352)  OP(368)  OP(384)  OP(400)  OP(416)  OP(432)   OP(448)  OP(464)  OP(480)  OP(496)
//            p += 512;
//        }
//        time_end = monotonic_time();
//        total_time += time_start - time_end;
//    }
//    return to_bw(TIMES * ARRAY_SIZE, total_time);
//}
//#undef OP;

int main(){
    // Have PAGE_SIZE buffering so we don't have to do math for prefetching.
    //ARRAY_SIZE 4MB + PAGE_SIZE 4KB


    int arr[ARRAY_SIZE+PAGE];

    double write, read;
    std::cout << 1;

    read = rd(arr,arr+ARRAY_SIZE+PAGE);
    //write = wr(arr,arr+ARRAY_SIZE+PAGE);
    std::cout << 2;

    printf("Read bandwidth: %.5f (GB/s); Write bandwidth: %.5f (GB/s)\n", read, write);
}

