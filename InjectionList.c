#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// Struct __Node
////////////////////////////////////////////////////////////////////////////////

typedef struct __Node
{
    struct __Node *__prev;
    struct __Node *__next;
} __Node;


////////////////////////////////////////////////////////////////////////////////
// Struct List
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    __Node __root;
} List;


////////////////////////////////////////////////////////////////////////////////
// List Constructor
////////////////////////////////////////////////////////////////////////////////

void listConstructor(List *this)
{
    this->__root.__prev = &this->__root;
    this->__root.__next = &this->__root;
}


////////////////////////////////////////////////////////////////////////////////
// List Empty
////////////////////////////////////////////////////////////////////////////////

_Bool listEmpty(List *this)
{
    return this->__root.__next == &this->__root;
}


////////////////////////////////////////////////////////////////////////////////
// List Push
////////////////////////////////////////////////////////////////////////////////

void listPush(List *this, __Node *nodePtr)
{
    nodePtr->__prev = this->__root.__prev;
    nodePtr->__next = &this->__root;

    this->__root.__prev->__next = nodePtr;
    this->__root.__prev         = nodePtr;
}


////////////////////////////////////////////////////////////////////////////////
// List Pop
////////////////////////////////////////////////////////////////////////////////

__Node *listPop(List *this)
{
    __Node *nodePtr = this->__root.__next;

    nodePtr->__prev->__next = nodePtr->__next;
    nodePtr->__next->__prev = nodePtr->__prev;

    return nodePtr;
}


////////////////////////////////////////////////////////////////////////////////
// List Destructor
////////////////////////////////////////////////////////////////////////////////

void listDestructor(List *this) {}


////////////////////////////////////////////////////////////////////////////////
// Macro Get Owner
////////////////////////////////////////////////////////////////////////////////

#define __getOwner(__OwnerType, __NodeField, __NodePtr) \
    ((__OwnerType *)((uintptr_t)(__NodePtr) - (uintptr_t)(&((__OwnerType *)0)->__NodeField)))


////////////////////////////////////////////////////////////////////////////////
// Struct Data
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    int    __data;
    __Node __node;
} Data;


////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

int main()
{
    List listObj;

    listConstructor(&listObj);

    Data dataObjA = {6}, dataObjB = {66}, dataObjC = {666};

    listPush(&listObj, &dataObjA.__node);
    listPush(&listObj, &dataObjB.__node);
    listPush(&listObj, &dataObjC.__node);

    while (!listEmpty(&listObj))
    {
        printf("%d\n", __getOwner(Data, __node, listPop(&listObj))->__data);
    }

    listDestructor(&listObj);
}
