//
// Created by imac on 2020-02-01.
//

#ifndef PA_EVAL_H
#define PA_EVAL_H

#include <iostream>
#include <cstdio>
#include <string>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>
#include <time.h>
#include <math.h>


class Eval {

public:

    int blocksize = 4*1024;
    int filesize;
    static uint64_t rdtsc(void);
    double readTimeOH();
    double loopOH();
    double sysCallOH();
    u_int64_t calculateSwitchTime(int *fd);
    double getContextSwitchTime();
    double getPageFault();

    double read_seq(char* file, void* buf);
    double read_rand(char* file, void* buf);

    void read_main();

};


#endif //PA_EVAL_H
