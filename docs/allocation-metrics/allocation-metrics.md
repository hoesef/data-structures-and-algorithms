# Allocation Metrics
AllocationMetrics is a utility structure used to track the amount of memory allocated to and freed from the heap. It is inteded to provide some insight into memory management and to help prevent / identify memory leaks. 

This struct simply stores 2 an integers (uint32_t), **totalAllocated** and **totalDeallocated**, and has a method **GetCurrentMemory** which returns the amount of memory still currently allocated.

The global *operator new* and *operator delete* functions are overwritten to update the values of totalAllocated and totalDeallocated respectively. 

## Why this works?
The *operator new* function is responsible for allocating a raw block of memory on the heap. Whenever we see code such as:
```cpp
int* x = new int[5];
```
what is essentially happening is the *operator new* function is being called to allocate a raw block of (sizeof(int) * 5) memory, then each of the 5 section of that memory block is being initialized as an integer using the int classe's constructor. 

By overloading the *operator new* function, we ensure that any time memory is allocated to the heap (be it by using the new keyword like in the example above, or by calling the *operator new* method directly) we are able to increment s_AllocationMetrics.totalAllocated.

Similarly, the *operator delete* function is called by the delte keyword, so by overloading the *operator delete* function we ensure that no matter how memory is deallocated, we get notified and can update s_AllocationMetrics.totalDeallocated accordingly.

## Aside
While this was mainly written to ensure that there were no memory leaks in my code, can also be a useful tool when looking to optimize code. If you are writting a performance critical program, this can help you notice that there are a lot of allocation and deallocations going on, which could suggest you might want to implement move semantics.

## Credit
Credit to The Cherno's video [Track MEMORY ALLOCATIONS the Easy Way in C++](https://www.youtube.com/watch?v=sLlGEUO_EGE&list=LL&index=1&pp=gAQBiAQB) for the AllocatioMetric structure and operator overloads. My code only adds in missing overloads for the array operator methods (operator new[], operator delete[], etc).