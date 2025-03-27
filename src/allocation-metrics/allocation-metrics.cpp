/*
 * Credit to The Cherno for the Allocation Metric idea
 * Video: Track MEMORY ALLOCATION the Easy Way in C++
 * Link: https://www.youtube.com/watch?v=sLlGEUO_EGE&list=LL&index=1&pp=gAQBiAQB
 */

 #include <iostream>

#include "..\..\include\allocation-metrics\allocation-metrics.h"

AllocationMetrics s_AllocationMetrics;

void* operator new(size_t size) {
    std::cout << "Allocating " << size << " bytes\n";
    s_AllocationMetrics.totalAllocated += size;
    return malloc(size);
}

void* operator new[](size_t size) {
    std::cout << "Allocating " << size << " bytes\n";
    s_AllocationMetrics.totalAllocated += size;
    return malloc(size);
}

void operator delete(void* memory, size_t size) {
    std::cout << "Freeing " << size << " bytes\n";
    s_AllocationMetrics.totalDeallocated += size;
    free(memory);
}

void operator delete[](void* memory, size_t size) {
    std::cout << "Freeing " << size << " bytes\n";
    s_AllocationMetrics.totalDeallocated += size;
    free(memory);
}

void operator delete(void* memory) {
    size_t size = _msize(memory);
    std::cout << "Freeing " << size << " bytes\n";
    s_AllocationMetrics.totalDeallocated += size;
    free(memory);
}

void operator delete[](void* memory) {
    size_t size = _msize(memory);
    std::cout << "Freeing " << size << " bytes\n";
    s_AllocationMetrics.totalDeallocated += size;
    free(memory);
}

void printCurrentMemory() {
    std::cout << "Current allocated memory: " << (s_AllocationMetrics.GetCurrentMemory()) << " bytes \n";
}
