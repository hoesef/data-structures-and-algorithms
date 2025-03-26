/*
 * Credit to The Cherno for the Allocation Metric idea
 * Video: Track MEMORY ALLOCATION the Easy Way in C++
 * Link: https://www.youtube.com/watch?v=sLlGEUO_EGE&list=LL&index=1&pp=gAQBiAQB
 */

#ifndef ALLOCATION_METRICS_H_
#define ALLOCATION_METRICS_H_

#include <cstdint>

struct AllocationMetrics {

    uint32_t totalAllocated = 0;
    uint32_t totalDeallocated = 0;
    uint32_t GetCurrentMemory() {return totalAllocated - totalDeallocated;}

};

extern AllocationMetrics s_AllocationMetrics;

void* operator new(size_t size);

void* operator new[](size_t size);

void operator delete(void* memory, size_t size);

void operator delete[](void* memory, size_t size);

void operator delete(void* memory);

void operator delete[](void* memory);

void printCurrentMemory();

#endif