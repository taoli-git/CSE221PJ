//
// Created by imac on 2020-02-01.
//


#include <fcntl.h>
#include <err.h>
#include "Eval.h"
#include <sys/types.h>
#include <sys/mman.h>
#include <setjmp.h>

#define N 1000000
#define CREAT_TIMES 10

using namespace std;
uint64_t Eval::rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

double Eval::readTimeOH() {
    double tsum=0;

    //rdtsc();
    for(int i=0;i<N;i++)
    {
        u_int64_t start = rdtsc();
        u_int64_t end = rdtsc();
        tsum += (end-start);
    }
    return (tsum/N);
}

double Eval::loopOH() {

    double tsum = 0;
    u_int64_t start = rdtsc();
    for(int i=0;i<N;i++){}
    u_int64_t end = rdtsc();
    tsum += (end - start);

    return tsum;
}


double Eval::sysCallOH() {
    double tsum=0;

    //rdtsc();
    for(int i=0;i<N;i++)
    {
        //rdtsc();
        u_int64_t start = rdtsc();
        getppid();
        u_int64_t end = rdtsc();
        tsum += (end-start);
    }
    return (tsum/N);
}

#define MAXPROCS 10
static struct {
    int valid;
    int pid;

    long long P;
    long long S;
    long long R;
} proctab[MAXPROCS];

int total = 0;
void res()
{
    int ct1=0;int left = 100 - total;
    for(int i=0;i<MAXPROCS;i++)
    {
        if(proctab[i].valid!=0)
        {
            proctab[i].P=0;
            if(proctab[i].R==0)
                ct1++, proctab[i].S=0;
            else
                proctab[i].S = proctab[i].R;
        }
    }

    if(ct1!=0 && left!=0)
    {
        for(int i=0;i<MAXPROCS;i++)
        {
            if(proctab[i].valid!=0 && proctab[i].R==0)
            {
                proctab[i].S = left/ct1;
            }
        }
    }
}
double Eval::getPageFault() {

    int fd = -1;
    if((fd = open("/Users/imac/Desktop/2020/CSE221/1.mp4", O_RDWR, 0))==-1)
    {
        err(1, "open");
    }

    long long offset = 1e9+7;
    long long fileSize = 1407366639;

    char* zero = (char*)mmap(NULL, fileSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    double sum = 0;
    uint64_t start,end;
    char ch;

    for(int i=0;i<10;i++)
    {
        start = rdtsc();
        ch = zero[(i*offset)%fileSize];
        end = rdtsc();
        sum+=(end-start);
    }

    munmap(zero, fileSize);
    close(fd);

    int ta=8,tc;

    start = rdtsc();
    tc = ta;
    end = rdtsc();
    return sum/10;

}

#define L 1e9
int pro()
{
    int ansid=-1;
    long long ans = 1e18+7;
    for(int i=0;i<MAXPROCS;i++)
    {
        if(proctab[i].valid != 0 && proctab[i].S != 0)
        {
            if(proctab[i].P < ans)
                ans = proctab[i].P, ansid = i;
        }
    }
    if(ans>1e15+7)
    {
        for(int i=0;i<MAXPROCS;i++) {
            if (proctab[i].valid != 0 && proctab[i].S != 0 && proctab[i].P > ans) {
                proctab[i].P -= ans;
            }
        }
    }

    if(ansid!=-1)
        proctab[ansid].P += L/proctab[ansid].S;
    return ansid;
}

double Eval::read_seq(char* file, void* buf) {

    int fd = open(file, O_SYNC);
    if(fcntl(fd, F_NOCACHE, 1) == -1) {
        printf("Failed.\n");
    }

    double sum = 0;
    uint64_t start,end;

    int len = filesize/blocksize;
    int t = len;
    while(t--)
    {
        start = rdtsc();
        int res = read(fd, buf, blocksize);

        end = rdtsc();
        sum+=(end - start);
    }

    close(fd);
    return sum/((double)len);
}

static int curThreadId = 0;
static int cntid = 0;

#define STACKSIZE	65536
#define MAXTHREADS 20
static struct thread {
    int valid;
    jmp_buf env;
    void (*func)();
    int p;
    int start;
} thread[MAXTHREADS], tmpthread[MAXTHREADS];
int qhead = -1, qend = -1;

int len  = 1005;
static struct node
{
    int v,pre,nxt;
}itm[1005];

void initq()
{
    for(int i=0;i<len;i++)
    {
        itm[i].pre = i-1;
        itm[i].nxt = i+1;
        if(i==len-1)
            itm[i].nxt = -1;
    }
}

void push(int id)
{
    //Printf("push %d\n",id);
    if(qend==-1)
    {
        qhead = qend = id;
        itm[id].pre = itm[id].nxt = -1;
    }else
    {
        itm[qend].nxt = id;
        itm[id].pre = qend;
        itm[id].nxt = -1;
        qend = id;
    }
}

int pop()
{
    int ans = qhead;
    itm[qhead].pre = itm[qhead].nxt = -1;
    qhead = itm[qhead].nxt;
    if(qhead!=-1)
        itm[qhead].pre = -1;
    return ans;
}

int remove(int id)
{
    //Printf("remove %d\n",id);

    if(qhead==id)
        qhead = itm[id].nxt;
    else if(itm[id].pre!=-1)
        itm[itm[id].pre].nxt = itm[id].nxt;

    if(qend==id)
        qend = itm[id].pre;
    else if(itm[id].nxt!=-1)
        itm[itm[id].nxt].pre = itm[id].pre;

    itm[id].pre = itm[id].nxt = -1;
    //Printf("remove %d\n",id);
}

double Eval::read_rand(char* file, void* buf) {

    int fd = open(file, O_SYNC);
    if(fcntl(fd, F_NOCACHE, 1) == -1) {
        printf("Failed.\n");
    }

    int len = filesize/blocksize;

    double sum = 0;
    uint64_t start,end;

    for(int i=0;i<len;i++)
    {
        int cur = rand()%len;
        start = rdtsc();
        lseek(fd, cur*blocksize, SEEK_SET);
        int res = read(fd, buf, blocksize);
        end = rdtsc();

        sum+=(end-start);
    }

    close(fd);
    return sum/((double)len);
}

void Eval::read_main()
{
    long long fs = 1024*1024;
    srand((unsigned int)time(NULL));
    void *buf = malloc(blocksize);

    char* filename = "/Users/imac/Desktop/2020/CSE221/1.mp4";
    for(int i=0;i<=7;i++)
    {
        filesize = fs;

        double ans1 = read_seq(filename, buf);
        double ans2 = read_rand(filename, buf);

        cout<<ans1/2600<<" "<<ans2/2600<<endl;

        fs*=2;
    }

    free(buf);
}