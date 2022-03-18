#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// Struct CopyOnWriteVector
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    int    *__data;
    size_t  __size;
    size_t  __cap;
    size_t *__ref;
} CopyOnWriteVector;


////////////////////////////////////////////////////////////////////////////////
// CopyOnWriteVector Constructor
////////////////////////////////////////////////////////////////////////////////

void copyOnWriteVectorConstructor(CopyOnWriteVector *this)
{
    this->__data = 0;
    this->__size = 0;
    this->__cap  = 0;
    this->__ref  = (size_t *)malloc(sizeof(size_t));

    *this->__ref = 1;
}


////////////////////////////////////////////////////////////////////////////////
// CopyOnWriteVector Destructor
////////////////////////////////////////////////////////////////////////////////

void copyOnWriteVectorDestructor(CopyOnWriteVector *this)
{
    if (!--*this->__ref)
    {
        free(this->__data);
        free(this->__ref);
    }
}


////////////////////////////////////////////////////////////////////////////////
// CopyOnWriteVector Copy
////////////////////////////////////////////////////////////////////////////////

void copyOnWriteVectorCopy(CopyOnWriteVector *tarPtr, CopyOnWriteVector *srcPtr)
{
    copyOnWriteVectorDestructor(tarPtr);

    *tarPtr = *srcPtr;

    ++*tarPtr->__ref;
}


////////////////////////////////////////////////////////////////////////////////
// CopyOnWriteVector Get
////////////////////////////////////////////////////////////////////////////////

int copyOnWriteVectorGet(CopyOnWriteVector *this, size_t idx)
{
    return this->__data[idx];
}


////////////////////////////////////////////////////////////////////////////////
// Disengage Memory
////////////////////////////////////////////////////////////////////////////////

void __disengageMemory(CopyOnWriteVector *this)
{
    int *newDataPtr = (int *)malloc(this->__cap * sizeof(int));

    memcpy(newDataPtr, this->__data, this->__size * sizeof(int));

    this->__data = newDataPtr;

    --*this->__ref;

    this->__ref = (size_t *)malloc(sizeof(size_t));

    *this->__ref = 1;
}


////////////////////////////////////////////////////////////////////////////////
// Try Copy
////////////////////////////////////////////////////////////////////////////////

void __tryCopy(CopyOnWriteVector *this, size_t idx, int val)
{
    if (this->__data[idx] != val && *this->__ref > 1)
    {
        __disengageMemory(this);
    }
}


////////////////////////////////////////////////////////////////////////////////
// CopyOnWriteVector Set
////////////////////////////////////////////////////////////////////////////////

void copyOnWriteVectorSet(CopyOnWriteVector *this, size_t idx, int val)
{
    __tryCopy(this, idx, val);

    this->__data[idx] = val;
}


////////////////////////////////////////////////////////////////////////////////
// Try Realloc
////////////////////////////////////////////////////////////////////////////////

void __tryRealloc(CopyOnWriteVector *this)
{
    if (this->__size == this->__cap)
    {
        this->__cap = (this->__cap + 1) * 2;

        if (*this->__ref == 1)
        {
            this->__data = (int *)realloc(this->__data, this->__cap * sizeof(int));
        }
        else
        {
            __disengageMemory(this);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// CopyOnWriteVector Push
////////////////////////////////////////////////////////////////////////////////

void copyOnWriteVectorPush(CopyOnWriteVector *this, int val)
{
    __tryRealloc(this);

    this->__data[this->__size++] = val;
}


////////////////////////////////////////////////////////////////////////////////
// CopyOnWriteVector Pop
////////////////////////////////////////////////////////////////////////////////

int copyOnWriteVectorPop(CopyOnWriteVector *this)
{
    return this->__data[--this->__size];
}


////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

int main()
{
    CopyOnWriteVector copyOnWriteVectorA, copyOnWriteVectorB;

    copyOnWriteVectorConstructor(&copyOnWriteVectorA);
    copyOnWriteVectorConstructor(&copyOnWriteVectorB);

    copyOnWriteVectorPush(&copyOnWriteVectorA, 0);
    copyOnWriteVectorPush(&copyOnWriteVectorA, 1);

    copyOnWriteVectorCopy(&copyOnWriteVectorB, &copyOnWriteVectorA);

    copyOnWriteVectorPop(&copyOnWriteVectorA);
    copyOnWriteVectorPush(&copyOnWriteVectorB, 6);

    printf("%d %d\n",
        copyOnWriteVectorGet(&copyOnWriteVectorA, 0),
        copyOnWriteVectorGet(&copyOnWriteVectorB, 2));

    copyOnWriteVectorCopy(&copyOnWriteVectorA, &copyOnWriteVectorB);

    copyOnWriteVectorSet(&copyOnWriteVectorA, 2, 666);

    printf("%d %d\n",
        copyOnWriteVectorGet(&copyOnWriteVectorA, 2),
        copyOnWriteVectorGet(&copyOnWriteVectorB, 2));

    copyOnWriteVectorDestructor(&copyOnWriteVectorA);
    copyOnWriteVectorDestructor(&copyOnWriteVectorB);
}
