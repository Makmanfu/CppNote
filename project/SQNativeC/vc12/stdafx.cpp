
#include "stdafx.h"

void* malloc(size_t size)
{
    return HeapAlloc(GetProcessHeap(), NULL, size);
}

void free(void* memblock)
{
    HeapFree(GetProcessHeap(), NULL, memblock);
}

void* realloc(void* memblock, size_t size)
{
    return HeapReAlloc(GetProcessHeap(), NULL, memblock, size);
}

void* operator new(size_t count)
{
    return malloc(count);
}

void* operator new[](size_t count)
{
    return malloc(count);
}

void operator delete(void* _Ptr) throw()
{
    free(_Ptr);
}

void  operator delete[](void* _Ptr) throw()
{
    free(_Ptr);
}



