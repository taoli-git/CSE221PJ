#include <iostream>
using namespace std;

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

double memory_latency(int arraySize, int strideSize)
{
    int **A;
    A = (int**)malloc(arraySize);
    int arrayLength = arraySize / sizeof(int *);
    int strideLength = strideSize / sizeof(int *);
    int index = 0;
    while(true) {
        int cur = index;
        index = index + strideLength;
        if (index>=arrayLength)
        {
            // construct a loop
            A[cur] = (int *) A;
            break;
        }
        A[cur] = (int *) (A+index);
    }

    int loads = 1000000;
    uint64_t start,end;
    start = rdtsc();
    int **p = A;
    for (int j = 0; j < loads; ++j) {
        p = (int **) *p;
    }
    end = rdtsc();
    double averageTime = (double)(end-start)/loads;
    free(A);
    return averageTime;
}

int main() {
    // unit for arraysize and stridesize: Byte
    for (int arraysize = 512; arraysize <= 1024 * 1024 * 256; arraysize *= 2) {
        //for (int stridesize = 8; stridesize <= 256*1024; stridesize *= 2) {
            int stridesize = 256;
            if (arraysize<stridesize) continue;
            cout << "arraysize is: " << arraysize << " stridesize is: " << 8 << " memory latency is: "
                 << memory_latency(arraysize, stridesize) << endl;
        //}
    }
    return 0;
}