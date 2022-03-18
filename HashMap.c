#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

////////////////////////////////////////////////////////////////////////////////
// Load Factor
////////////////////////////////////////////////////////////////////////////////

#define __LOAD_FACTOR 0.75


////////////////////////////////////////////////////////////////////////////////
// Struct __Node
////////////////////////////////////////////////////////////////////////////////

typedef struct __Node
{
    int            __key;
    int            __value;
    struct __Node *__next;
} __Node;


////////////////////////////////////////////////////////////////////////////////
// Struct HashMap
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    __Node *__barrel;
    size_t  __size;
    size_t  __cap;
} HashMap;


////////////////////////////////////////////////////////////////////////////////
// Int Hash Function
////////////////////////////////////////////////////////////////////////////////

size_t __intHashFunction(int key)
{
    return key;
}


////////////////////////////////////////////////////////////////////////////////
// HashMap Constructor
////////////////////////////////////////////////////////////////////////////////

void hashMapConstructor(HashMap *this)
{
    this->__barrel = 0;
    this->__size   = 0;
    this->__cap    = 0;
}


////////////////////////////////////////////////////////////////////////////////
// HashMap Destructor
////////////////////////////////////////////////////////////////////////////////

void hashMapDestructor(HashMap *this)
{
    for (size_t barrelIdx = 0; barrelIdx < this->__cap; barrelIdx++)
    {
        __Node *nodePtr = this->__barrel[barrelIdx].__next;

        while (nodePtr)
        {
            __Node *nextNodePtr = nodePtr->__next;

            free(nodePtr);

            nodePtr = nextNodePtr;
        }
    }

    free(this->__barrel);
}


////////////////////////////////////////////////////////////////////////////////
// HashMap Find
////////////////////////////////////////////////////////////////////////////////

int *hashMapFind(HashMap *this, int key)
{
    if (!this->__cap)
    {
        return 0;
    }

    for (__Node *nodePtr = this->__barrel + __intHashFunction(key) % this->__cap;
        nodePtr->__next;
        nodePtr = nodePtr->__next)
    {
        if (nodePtr->__next->__key == key)
        {
            return &nodePtr->__next->__value;
        }
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
// HashMap Rehash
////////////////////////////////////////////////////////////////////////////////

void __hashMapRehash(HashMap *this)
{
    size_t newCap     = (this->__cap + 1) * 2;
    __Node *newBarrel = (__Node *)malloc(sizeof(__Node) * newCap);

    for (size_t barrelIdx = 0; barrelIdx < newCap; barrelIdx++)
    {
        newBarrel[barrelIdx].__next = 0;
    }

    for (int barrelIdx = 0; barrelIdx < this->__cap; barrelIdx++)
    {
        for (__Node *oldNodePtr = this->__barrel + barrelIdx;
            oldNodePtr->__next;
            oldNodePtr = oldNodePtr->__next)
        {
            __Node *newNodePtr = newBarrel + __intHashFunction(oldNodePtr->__next->__key) % newCap;

            while (1)
            {
                if (newNodePtr->__next)
                {
                    newNodePtr = newNodePtr->__next;
                }
                else
                {
                    newNodePtr->__next = (__Node *)malloc(sizeof(__Node));

                    newNodePtr->__next->__key   = oldNodePtr->__next->__key;
                    newNodePtr->__next->__value = oldNodePtr->__next->__value;
                    newNodePtr->__next->__next  = 0;

                    break;
                }
            }
        }
    }

    hashMapDestructor(this);

    this->__barrel = newBarrel;
    this->__cap    = newCap;
}


////////////////////////////////////////////////////////////////////////////////
// HashMap Insert
////////////////////////////////////////////////////////////////////////////////

void hashMapInsert(HashMap *this, int key, int value)
{
    if (this->__size >= this->__cap * __LOAD_FACTOR)
    {
        __hashMapRehash(this);
    }

    __Node *nodePtr = this->__barrel + __intHashFunction(key) % this->__cap;

    for (; nodePtr->__next; nodePtr = nodePtr->__next)
    {
        if (nodePtr->__next->__key == key)
        {
            nodePtr->__next->__value = value;

            return;
        }
    }

    nodePtr->__next = (__Node *)malloc(sizeof(__Node));

    nodePtr->__next->__key   = key;
    nodePtr->__next->__value = value;
    nodePtr->__next->__next  = 0;

    this->__size++;
}


////////////////////////////////////////////////////////////////////////////////
// HashMap Erase
////////////////////////////////////////////////////////////////////////////////

void hashMapErase(HashMap *this, int key)
{
    if (!this->__cap)
    {
        return;
    }

    for (__Node *nodePtr = this->__barrel + __intHashFunction(key) % this->__cap;
        nodePtr->__next;
        nodePtr = nodePtr->__next)
    {
        if (nodePtr->__next->__key == key)
        {
            __Node *eraseNodePtr = nodePtr->__next;

            nodePtr->__next = eraseNodePtr->__next;

            free(eraseNodePtr);

            this->__size--;

            break;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

int main()
{
    HashMap hashMapObj;

    hashMapConstructor(&hashMapObj);

    hashMapInsert(&hashMapObj, 1, 6);
    hashMapInsert(&hashMapObj, 2, 5);
    hashMapInsert(&hashMapObj, 3, 4);
    hashMapInsert(&hashMapObj, 4, 3);
    hashMapInsert(&hashMapObj, 5, 2);
    hashMapInsert(&hashMapObj, 6, 1);

    hashMapErase(&hashMapObj, 1);
    hashMapErase(&hashMapObj, 3);
    hashMapErase(&hashMapObj, 5);

    *hashMapFind(&hashMapObj, 6) = 6;

    for (int key = 2; key <= 6; key += 2)
    {
        printf("%d: %d\n", key, *hashMapFind(&hashMapObj, key));
    }

    hashMapDestructor(&hashMapObj);
}
