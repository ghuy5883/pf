#include "stdafx.h"
#include "engineMemory.h"


void *AlignedMalloc(size_t size, size_t align)
{
    // TODO Lab 01k
    return malloc(size);
}

void AlignedFree(void *inMem)
{
    // TODO Lab 01k
    return free(inMem);
}

bool AlignedMalloc_UnitTest()
{
    bool isOk = true;

    struct MallocTest
    {
        size_t size;
        size_t align;
    };
    MallocTest mallocTest[] =
    {
        { 100, 16 },
        { 200, 16 },
        { 256, 16 },
        { 1024, 16 },
        { 10, 256 },
        { 256, 256 },
        { 511, 256 },
        { 512, 256 },
        { 513, 256 },
        { 100, 1 },
    };
    int count = sizeof(mallocTest) / sizeof(mallocTest[0]);

    // allocate everything in the mallocTest table
    void **pointers = new void*[count];
    for (int i = 0; i < count; ++i)
    {
        // allocate the pointer
        pointers[i] = AlignedMalloc(mallocTest[i].size, mallocTest[i].align);
        //make sure it came out aligned
        uintptr_t address = reinterpret_cast<uintptr_t>(pointers[i]);
        isOk &= (address & (mallocTest[i].align - 1)) == 0;
        // fill the memory at that location with the index
        memset(pointers[i], i, mallocTest[i].size);
    }
    // release all the memory
    for (int i = 0; i < count; ++i)
    {
        // first make sure that the memory still contains what you put in there
        for (size_t j = 0; j < mallocTest[i].size; ++j)
        {
            char* pChar = reinterpret_cast<char*>(pointers[i]);
            isOk &= pChar[j] == i;
        }
        // and free the memory
        AlignedFree(pointers[i]);
    }
    delete[] pointers;

    return isOk;
}