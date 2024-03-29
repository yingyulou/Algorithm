#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// Struct Node
////////////////////////////////////////////////////////////////////////////////

typedef struct __Node
{
    int __val;
    struct __Node *__prev;
    struct __Node *__next;
} Node;


////////////////////////////////////////////////////////////////////////////////
// Struct List
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    Node __root;
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
// List Begin
////////////////////////////////////////////////////////////////////////////////

Node *listBegin(List *this)
{
    return this->__root.__next;
}


////////////////////////////////////////////////////////////////////////////////
// List End
////////////////////////////////////////////////////////////////////////////////

Node *listEnd(List *this)
{
    return &this->__root;
}


////////////////////////////////////////////////////////////////////////////////
// List Next
////////////////////////////////////////////////////////////////////////////////

Node *listNext(Node *this)
{
    return this->__next;
}


////////////////////////////////////////////////////////////////////////////////
// List Get
////////////////////////////////////////////////////////////////////////////////

int listGet(Node *this)
{
    return this->__val;
}


////////////////////////////////////////////////////////////////////////////////
// List Set
////////////////////////////////////////////////////////////////////////////////

void listSet(Node *this, int val)
{
    this->__val = val;
}


////////////////////////////////////////////////////////////////////////////////
// List Insert
////////////////////////////////////////////////////////////////////////////////

void listInsert(Node *this, int val)
{
    Node *newNode = (Node *)malloc(sizeof(Node));

    newNode->__val  = val;
    newNode->__prev = this->__prev;
    newNode->__next = this;

    this->__prev->__next = newNode;
    this->__prev         = newNode;
}


////////////////////////////////////////////////////////////////////////////////
// List Erase
////////////////////////////////////////////////////////////////////////////////

void listErase(Node *this)
{
    this->__prev->__next = this->__next;
    this->__next->__prev = this->__prev;

    free(this);
}


////////////////////////////////////////////////////////////////////////////////
// List Destructor
////////////////////////////////////////////////////////////////////////////////

void listDestructor(List *this)
{
    Node *curNodePtr = this->__root.__next;

    while (curNodePtr != &this->__root)
    {
        Node *nextNodePtr = curNodePtr->__next;

        free(curNodePtr);

        curNodePtr = nextNodePtr;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

int main()
{
    List listObj;

    listConstructor(&listObj);

    listInsert(listEnd(&listObj), 1);
    listInsert(listEnd(&listObj), 2);
    listInsert(listEnd(&listObj), 3);

    listErase(listNext(listBegin(&listObj)));

    listSet(listBegin(&listObj), 4);

    for (Node *nodePtr = listBegin(&listObj);
        nodePtr != listEnd(&listObj);
        nodePtr = listNext(nodePtr))
    {
        printf("%d\n", listGet(nodePtr));
    }

    listDestructor(&listObj);
}
