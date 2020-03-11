#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>      //pipe()
#include <thread>
using namespace std;

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

uint64_t contextSwitchCalc(int *fd)
{
    uint64_t start,end;
    uint64_t res = 0;
    pid_t pid;
    if ((pid = fork())!=0)
    {
        start = rdtsc();

        wait(NULL);

        read(fd[0],(void *)&end, sizeof(end));

    } else{
        end = rdtsc();
        write(fd[1],(void *)&end, sizeof(end));
        exit(1);
    }
    if (end > start)
    {
        res = end - start;
    }
    return res;

}

double processCS(){
    double sum = 0;
    int count = 0;
    int fd[2];
    pipe(fd);
    for (int i = 0; i < 500; i++) {
        uint64_t t = contextSwitchCalc(fd);
        if (t > 0) {
            sum += t;
            count++;
        }
    }
    return sum/500;
}

void helper(int *fd)
{
    uint64_t now = rdtsc();
    write(fd[1],(void *)&now, sizeof(uint64_t));
}

uint64_t threadContextSwitchCalc()
{
    int fd[2];
    pipe(fd);
    thread t(helper,fd);
    uint64_t st = rdtsc();
    t.join(); //context switch
    uint64_t ed;
    read(fd[0],(void *)&ed, sizeof(uint64_t));
    cout << ed - st <<endl;
}

double processCreationTime()
{
    uint64_t st;
    uint64_t ed;
    double sum = 0;
    pid_t pid;
    for (int i=0;i<500;i++)
    {
        st = rdtsc();
        if ((pid = fork())==0)
        {
            exit(1);
        } else{
            wait(NULL);
            ed = rdtsc();
            sum += ed - st;
        }
    }
    return sum/500;
}

void td(){}

double threadCreationTime()
{
    uint64_t st;
    uint64_t ed;
    double sum = 0;
    for (int i=0;i<500;i++)
    {
        st = rdtsc();
        thread t(td);
        t.join();
        ed = rdtsc();
        sum += ed - st;
    }
    return sum/500;
}

int main() {
    threadContextSwitchCalc();
    return 0;
}