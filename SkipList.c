#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
// Struct __Node
////////////////////////////////////////////////////////////////////////////////

typedef struct __Node
{
    int            __key;
    void          *__value;
    struct __Node *__next;
} __Node;


////////////////////////////////////////////////////////////////////////////////
// __Node Constructor
////////////////////////////////////////////////////////////////////////////////

void __nodeConstructor(__Node *this, int key, void *value, __Node *next)
{
    this->__key   = key;
    this->__value = value;
    this->__next  = next;
}


////////////////////////////////////////////////////////////////////////////////
// Struct SkipList
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    __Node *__root;
    size_t  __layer;
} SkipList;


////////////////////////////////////////////////////////////////////////////////
// SkipList Constructor
////////////////////////////////////////////////////////////////////////////////

void skipListConstructor(SkipList *this, size_t layer)
{
    #ifdef NDEBUG
    srand(time(0));
    #else
    srand(6);
    #endif

    this->__root  = (__Node *)malloc(sizeof(__Node));
    this->__layer = layer;

    __nodeConstructor(this->__root, 0, 0, 0);

    __Node *upNodePtr = this->__root;

    for (size_t _ = 0; _ < layer - 1; _++)
    {
        upNodePtr->__value = malloc(sizeof(__Node));

        __nodeConstructor((__Node *)upNodePtr->__value, 0, 0, 0);

        upNodePtr = (__Node *)upNodePtr->__value;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Find Left Node
////////////////////////////////////////////////////////////////////////////////

__Node *__findLeftNode(__Node *nodePtr, int key, _Bool forceBool)
{
    for (; nodePtr->__next && nodePtr->__next->__key < key;
        nodePtr = nodePtr->__next);

    if ((!nodePtr->__next || nodePtr->__next->__key != key) && forceBool)
    {
        __Node *newNodePtr = (__Node *)malloc(sizeof(__Node));

        __nodeConstructor(newNodePtr, key, 0, nodePtr->__next);

        nodePtr->__next = newNodePtr;
    }

    return nodePtr;
}


////////////////////////////////////////////////////////////////////////////////
// SkipList Find
////////////////////////////////////////////////////////////////////////////////

int *skipListFind(SkipList *this, int key)
{
    __Node *nodePtr = this->__root;

    for (size_t curLayer = 0; curLayer < this->__layer; curLayer++)
    {
        nodePtr = __findLeftNode(nodePtr, key, 0);

        if (nodePtr->__next && nodePtr->__next->__key == key)
        {
            for (nodePtr = nodePtr->__next; curLayer < this->__layer - 1;
                curLayer++, nodePtr = (__Node *)nodePtr->__value);

            return (int *)nodePtr->__value;
        }
        else
        {
            nodePtr = (__Node *)nodePtr->__value;
        }
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
// Roulette Test
////////////////////////////////////////////////////////////////////////////////

_Bool __rouletteTest(SkipList *this, size_t curLayer)
{
    return rand() / (double)RAND_MAX <= 1. / (1 << (this->__layer - curLayer - 1));
}


////////////////////////////////////////////////////////////////////////////////
// SkipList Insert Helper
////////////////////////////////////////////////////////////////////////////////

void __skipListInsertHelper(SkipList *this, int key, int value,
    __Node *nodePtr, size_t curLayer)
{
    __Node *upLeftNodePtr = __findLeftNode(nodePtr, key, 1);

    for (; curLayer < this->__layer - 1; curLayer++)
    {
        __Node *downLeftNodePtr = __findLeftNode((__Node *)upLeftNodePtr->__value, key, 1);

        upLeftNodePtr->__next->__value = downLeftNodePtr->__next;

        upLeftNodePtr = downLeftNodePtr;
    }

    if (!upLeftNodePtr->__next->__value)
    {
        upLeftNodePtr->__next->__value = malloc(sizeof(int));
    }

    *(int *)upLeftNodePtr->__next->__value = value;
}


////////////////////////////////////////////////////////////////////////////////
// SkipList Insert
////////////////////////////////////////////////////////////////////////////////

void skipListInsert(SkipList *this, int key, int value)
{
    __Node *nodePtr = this->__root;

    for (size_t curLayer = 0; curLayer < this->__layer; curLayer++)
    {
        nodePtr = __findLeftNode(nodePtr, key, 0);

        if (nodePtr->__next && nodePtr->__next->__key == key)
        {
            for (nodePtr = nodePtr->__next; curLayer < this->__layer - 1;
                curLayer++, nodePtr = (__Node *)nodePtr->__value);

            *(int *)nodePtr->__value = value;
        }
        else if (__rouletteTest(this, curLayer))
        {
            __skipListInsertHelper(this, key, value, nodePtr, curLayer);

            break;
        }
        else
        {
            nodePtr = (__Node *)nodePtr->__value;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// SkipList Erase
////////////////////////////////////////////////////////////////////////////////

void skipListErase(SkipList *this, int key)
{
    __Node *nodePtr = this->__root;

    for (size_t curLayer = 0; curLayer < this->__layer; curLayer++)
    {
        nodePtr = __findLeftNode(nodePtr, key, 0);

        if (nodePtr->__next && nodePtr->__next->__key == key)
        {
            __Node *freeNodePtr = nodePtr->__next;

            nodePtr->__next = freeNodePtr->__next;

            if (curLayer == this->__layer - 1)
            {
                free(freeNodePtr->__value);
            }

            free(freeNodePtr);
        }

        nodePtr = (__Node *)nodePtr->__value;
    }
}


////////////////////////////////////////////////////////////////////////////////
// SkipList Destructor
////////////////////////////////////////////////////////////////////////////////

void __skipListDestructorHelper(__Node *nodePtr, _Bool lastLayerBool)
{
    __Node *nextNodePtr;

    if (lastLayerBool)
    {
        for (; nodePtr; nextNodePtr = nodePtr->__next, free(nodePtr->__value),
            free(nodePtr), nodePtr = nextNodePtr);
    }
    else
    {
        for (; nodePtr; nextNodePtr = nodePtr->__next, free(nodePtr),
            nodePtr = nextNodePtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// SkipList Destructor
////////////////////////////////////////////////////////////////////////////////

void skipListDestructor(SkipList *this)
{
    for (__Node *upNodePtr = this->__root, *downNodePtr; upNodePtr;
        downNodePtr = (__Node *)upNodePtr->__value,
        __skipListDestructorHelper(upNodePtr, !downNodePtr),
        upNodePtr = downNodePtr);
}


////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

int main()
{
    SkipList skipListObj;

    skipListConstructor(&skipListObj, 3);

    for (int key = 0; key < 10; key++)
    {
        skipListInsert(&skipListObj, key, key);
    }

    skipListErase(&skipListObj, 0);
    skipListErase(&skipListObj, 7);
    skipListErase(&skipListObj, 9);

    *skipListFind(&skipListObj, 6) = 666;

    printf("%d\n", *skipListFind(&skipListObj, 6));

    skipListDestructor(&skipListObj);
}
