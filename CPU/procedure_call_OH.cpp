#include <iostream>
#include <vector>
#include <fstream>


static inline  uint64_t rdtsc() //unsigned long long
{
    uint32_t lo,hi;

    __asm__ __volatile__
    (
    "rdtsc":"=a"(lo),"=d"(hi)
    );
    return (((uint64_t)hi)<<32|lo);
}


static inline void proc0(){
}

static inline void proc1(int a){
}

static inline void proc2(int a, int b){
}

static inline void proc3(int a, int b, int c){
}

static inline void proc4(int a, int b, int c, int d){
}

static inline void proc5(int a, int b, int c, int d, int e){
}

static inline void proc6(int a, int b, int c, int d, int e, int f){
}

static inline void proc7(int a, int b, int c, int d, int e, int f, int g){
}

void procCall (std::vector<double> &res){

    int ITERATION = 10000;

    uint64_t time = 0;
    for(int i = 0; i< ITERATION; i++){
        uint64_t start = rdtsc();
        proc0();
        uint64_t end = rdtsc();
        time += end-start;
    }
    res[0] = (double )time/(double)ITERATION;

    rdtsc();
    time = 0;
    for(int i = 0; i< ITERATION; i++){
        uint64_t start = rdtsc();
        proc1(1);
        uint64_t end = rdtsc();
        time += end-start;
    }
    res[1] = (double )time/(double)ITERATION;

    rdtsc();
    time = 0;
    for(int i = 0; i < ITERATION; i++){
        uint64_t start = rdtsc();
        proc2(1,1);
        uint64_t end = rdtsc();
        time += end-start;
    }
    res[2] = (double )time/(double)ITERATION;

    rdtsc();
    time = 0;
    for(int i = 0; i < ITERATION; i++){
        uint64_t start = rdtsc();
        proc3(1,1,1);
        uint64_t end = rdtsc();
        time += end-start;
    }
    res[3] = (double )time/(double)ITERATION;

    rdtsc();
    time = 0;
    for(int i = 0; i < ITERATION; i++){
        uint64_t start = rdtsc();
        proc4(1,1,1,1);
        uint64_t end = rdtsc();
        time += end-start;
    }
    res[4] = (double )time/(double)ITERATION;

    rdtsc();
    time = 0;
    for(int i = 0; i < ITERATION; i++){
        uint64_t start = rdtsc();
        proc5(1,1,1,1,1);
        uint64_t end = rdtsc();
        time += end-start;
    }
    res[5] = (double )time/(double)ITERATION;

    rdtsc();
    time = 0;
    for(int i = 0; i < ITERATION; i++){
        uint64_t start = rdtsc();
        proc6(1,1,1,1,1,1);
        uint64_t end = rdtsc();
        time += end-start;
    }
    res[6] = (double )time/(double)ITERATION;

    rdtsc();
    time = 0;
    for(int i = 0; i < ITERATION; i++){
        uint64_t start = rdtsc();
        proc7(1,1,1,1,1,1,1);
        uint64_t end = rdtsc();
        time += end-start;
    }
    res[7] = (double )time/(double)ITERATION;
}


int main() {
    std::ofstream output_file("./procCall.txt");

    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    for (int i = 0; i< 10; i++){
        std::vector<double> res(8,0.);
        procCall(res);
        std::cout << "Hello, World!" << std::endl;
        for(unsigned int i=0; i<res.size()-1; i++)
            output_file << res[i] << " ";
        output_file << res[res.size()-1] << "\n";
    }
    output_file.close();
    //std::cout << res[7] << std::endl;
    //std::copy(res.begin(), res.end(), output_iterator);
    return 0;
}