#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

////////////////////////////////////////////////////////////////////////////////
// Enum __Color
////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    __Red,
    __Black,
} __Color;


////////////////////////////////////////////////////////////////////////////////
// Struct __Node
////////////////////////////////////////////////////////////////////////////////

typedef struct __Node
{
    int            __key;
    int            __value;
    struct __Node *__owner;
    struct __Node *__left;
    struct __Node *__right;
    __Color        __color;
} __Node;


////////////////////////////////////////////////////////////////////////////////
// Nil Node
////////////////////////////////////////////////////////////////////////////////

__Node __NIL = {0, 0, 0, 0, 0, __Black};


////////////////////////////////////////////////////////////////////////////////
// __Node Constructor
////////////////////////////////////////////////////////////////////////////////

void __nodeConstructor(__Node *this, int key, int value, __Node *owner)
{
    this->__key   = key;
    this->__value = value;
    this->__owner = owner;
    this->__left  = &__NIL;
    this->__right = &__NIL;
    this->__color = __Red;
}


////////////////////////////////////////////////////////////////////////////////
// Struct RBTree
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    __Node *__root;
} RBTree;


////////////////////////////////////////////////////////////////////////////////
// RBTree Constructor
////////////////////////////////////////////////////////////////////////////////

void rbTreeConstructor(RBTree *this)
{
    this->__root = &__NIL;
}


////////////////////////////////////////////////////////////////////////////////
// RBTree Find
////////////////////////////////////////////////////////////////////////////////

int *rbTreeFind(RBTree *this, int key)
{
    for (__Node *nodePtr = this->__root; nodePtr != &__NIL; /* See below */)
    {
        if (key < nodePtr->__key)
        {
            nodePtr = nodePtr->__left;
        }
        else if (key > nodePtr->__key)
        {
            nodePtr = nodePtr->__right;
        }
        else
        {
            return &nodePtr->__value;
        }
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
// Rotate Left
////////////////////////////////////////////////////////////////////////////////

void __rotateLeft(RBTree *this, __Node *nodePtr)
{
    __Node *rightNodePtr = nodePtr->__right;

    if (nodePtr == this->__root)
    {
        this->__root = rightNodePtr;
    }
    else
    {
        if (nodePtr == nodePtr->__owner->__left)
        {
            nodePtr->__owner->__left = rightNodePtr;
        }
        else
        {
            nodePtr->__owner->__right = rightNodePtr;
        }
    }

    rightNodePtr->__owner = nodePtr->__owner;

    nodePtr->__right = rightNodePtr->__left;
    rightNodePtr->__left->__owner = nodePtr;

    rightNodePtr->__left = nodePtr;
    nodePtr->__owner = rightNodePtr;
}


////////////////////////////////////////////////////////////////////////////////
// Rotate Right
////////////////////////////////////////////////////////////////////////////////

void __rotateRight(RBTree *this, __Node *nodePtr)
{
    __Node *leftNodePtr = nodePtr->__left;

    if (nodePtr == this->__root)
    {
        this->__root = leftNodePtr;
    }
    else
    {
        if (nodePtr == nodePtr->__owner->__left)
        {
            nodePtr->__owner->__left = leftNodePtr;
        }
        else
        {
            nodePtr->__owner->__right = leftNodePtr;
        }
    }

    leftNodePtr->__owner = nodePtr->__owner;

    nodePtr->__left = leftNodePtr->__right;
    leftNodePtr->__right->__owner = nodePtr;

    leftNodePtr->__right = nodePtr;
    nodePtr->__owner = leftNodePtr;
}


////////////////////////////////////////////////////////////////////////////////
// RBTree Repair Insert
////////////////////////////////////////////////////////////////////////////////

void __rbTreeRepairInsert(RBTree *this, __Node *nodePtr)
{
    while (nodePtr->__owner->__color == __Red)
    {
        if (nodePtr->__owner == nodePtr->__owner->__owner->__left)
        {
            __Node *brotherNodePtr = nodePtr->__owner->__owner->__right;

            if (brotherNodePtr->__color == __Red)
            {
                nodePtr->__owner->__color          = __Black;
                brotherNodePtr->__color            = __Black;
                nodePtr->__owner->__owner->__color = __Red;

                nodePtr = nodePtr->__owner->__owner;
            }
            else
            {
                if (nodePtr == nodePtr->__owner->__left)
                {
                    nodePtr->__owner->__color          = __Black;
                    nodePtr->__owner->__owner->__color = __Red;

                    __rotateRight(this, nodePtr->__owner->__owner);
                }
                else
                {
                    __rotateLeft(this, nodePtr->__owner);

                    nodePtr->__color          = __Black;
                    nodePtr->__owner->__color = __Red;

                    __rotateRight(this, nodePtr->__owner);
                }

                break;
            }
        }
        else
        {
            __Node *brotherNodePtr = nodePtr->__owner->__owner->__left;

            if (brotherNodePtr->__color == __Red)
            {
                nodePtr->__owner->__color          = __Black;
                brotherNodePtr->__color            = __Black;
                nodePtr->__owner->__owner->__color = __Red;

                nodePtr = nodePtr->__owner->__owner;
            }
            else
            {
                if (nodePtr == nodePtr->__owner->__right)
                {
                    nodePtr->__owner->__color          = __Black;
                    nodePtr->__owner->__owner->__color = __Red;

                    __rotateLeft(this, nodePtr->__owner->__owner);
                }
                else
                {
                    __rotateRight(this, nodePtr->__owner);

                    nodePtr->__color          = __Black;
                    nodePtr->__owner->__color = __Red;

                    __rotateLeft(this, nodePtr->__owner);
                }

                break;
            }
        }
    }

    this->__root->__color = __Black;
}


////////////////////////////////////////////////////////////////////////////////
// RBTree Insert
////////////////////////////////////////////////////////////////////////////////

void rbTreeInsert(RBTree *this, int key, int value)
{
    if (this->__root == &__NIL)
    {
        this->__root = (__Node *)malloc(sizeof(__Node));

        __nodeConstructor(this->__root, key, value, &__NIL);

        this->__root->__color = __Black;
    }
    else
    {
        __Node *nodePtr = this->__root;

        while (1)
        {
            if (key < nodePtr->__key)
            {
                if (nodePtr->__left != &__NIL)
                {
                    nodePtr = nodePtr->__left;
                }
                else
                {
                    nodePtr->__left = (__Node *)malloc(sizeof(__Node));

                    __nodeConstructor(nodePtr->__left, key, value, nodePtr);

                    __rbTreeRepairInsert(this, nodePtr->__left);

                    break;
                }
            }
            else if (key > nodePtr->__key)
            {
                if (nodePtr->__right != &__NIL)
                {
                    nodePtr = nodePtr->__right;
                }
                else
                {
                    nodePtr->__right = (__Node *)malloc(sizeof(__Node));

                    __nodeConstructor(nodePtr->__right, key, value, nodePtr);

                    __rbTreeRepairInsert(this, nodePtr->__right);

                    break;
                }
            }
            else
            {
                nodePtr->__value = value;

                break;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// Find Delete Node
////////////////////////////////////////////////////////////////////////////////

__Node *__findDeleteNode(RBTree *this, int key)
{
    __Node *nodePtr = this->__root;

    while (nodePtr != &__NIL)
    {
        if (key < nodePtr->__key)
        {
            nodePtr = nodePtr->__left;
        }
        else if (key > nodePtr->__key)
        {
            nodePtr = nodePtr->__right;
        }
        else
        {
            break;
        }
    }

    return nodePtr;
}


////////////////////////////////////////////////////////////////////////////////
// Transfer Delete Node
////////////////////////////////////////////////////////////////////////////////

void __transferDeleteNode(__Node **nodePtrPtr)
{
    __Node *replaceNodePtr = (*nodePtrPtr)->__left;

    for (; replaceNodePtr->__right != &__NIL;
        replaceNodePtr = replaceNodePtr->__right);

    (*nodePtrPtr)->__key   = replaceNodePtr->__key;
    (*nodePtrPtr)->__value = replaceNodePtr->__value;

    *nodePtrPtr = replaceNodePtr;
}


////////////////////////////////////////////////////////////////////////////////
// RBTree Repair Erase
////////////////////////////////////////////////////////////////////////////////

void __rbTreeRepairErase(RBTree *this, __Node *nodePtr)
{
    while (nodePtr->__color == __Black && nodePtr != this->__root)
    {
        if (nodePtr == nodePtr->__owner->__left)
        {
            __Node *brotherNodePtr = nodePtr->__owner->__right;

            if (brotherNodePtr->__color == __Red)
            {
                brotherNodePtr->__color          = __Black;
                brotherNodePtr->__owner->__color = __Red;

                __rotateLeft(this, brotherNodePtr->__owner);

                brotherNodePtr = nodePtr->__owner->__right;
            }

            if (brotherNodePtr->__left->__color == __Black &&
                brotherNodePtr->__right->__color == __Black)
            {
                brotherNodePtr->__color = __Red;

                nodePtr = nodePtr->__owner;
            }
            else
            {
                if (brotherNodePtr->__right->__color == __Black)
                {
                    brotherNodePtr->__color         = __Red;
                    brotherNodePtr->__left->__color = __Black;

                    __rotateRight(this, brotherNodePtr);

                    brotherNodePtr = nodePtr->__owner->__right;
                }

                brotherNodePtr->__color          = brotherNodePtr->__owner->__color;
                brotherNodePtr->__owner->__color = __Black;
                brotherNodePtr->__right->__color = __Black;

                __rotateLeft(this, brotherNodePtr->__owner);

                nodePtr = this->__root;
            }
        }
        else
        {
            __Node *brotherNodePtr = nodePtr->__owner->__left;

            if (brotherNodePtr->__color == __Red)
            {
                brotherNodePtr->__color          = __Black;
                brotherNodePtr->__owner->__color = __Red;

                __rotateRight(this, brotherNodePtr->__owner);

                brotherNodePtr = nodePtr->__owner->__left;
            }

            if (brotherNodePtr->__left->__color == __Black &&
                brotherNodePtr->__right->__color == __Black)
            {
                brotherNodePtr->__color = __Red;

                nodePtr = nodePtr->__owner;
            }
            else
            {
                if (brotherNodePtr->__left->__color == __Black)
                {
                    brotherNodePtr->__color          = __Red;
                    brotherNodePtr->__right->__color = __Black;

                    __rotateLeft(this, brotherNodePtr);

                    brotherNodePtr = nodePtr->__owner->__left;
                }

                brotherNodePtr->__color          = brotherNodePtr->__owner->__color;
                brotherNodePtr->__owner->__color = __Black;
                brotherNodePtr->__left->__color  = __Black;

                __rotateRight(this, brotherNodePtr->__owner);

                nodePtr = this->__root;
            }
        }
    }

    nodePtr->__color = __Black;
}


////////////////////////////////////////////////////////////////////////////////
// RBTree Erase
////////////////////////////////////////////////////////////////////////////////

void rbTreeErase(RBTree *this, int key)
{
    __Node *nodePtr = __findDeleteNode(this, key);

    if (nodePtr != &__NIL)
    {
        if (nodePtr->__left != &__NIL && nodePtr->__right != &__NIL)
        {
            __transferDeleteNode(&nodePtr);
        }

        __Node *replaceNodePtr = nodePtr->__left != &__NIL ?
            nodePtr->__left : nodePtr->__right;

        replaceNodePtr->__owner = nodePtr->__owner;

        if (nodePtr == this->__root)
        {
            this->__root = replaceNodePtr;
        }
        else if (nodePtr == nodePtr->__owner->__left)
        {
            nodePtr->__owner->__left = replaceNodePtr;
        }
        else
        {
            nodePtr->__owner->__right = replaceNodePtr;
        }

        if (nodePtr->__color == __Black)
        {
            __rbTreeRepairErase(this, replaceNodePtr);
        }

        free(nodePtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// RBTree Destructor Helper
////////////////////////////////////////////////////////////////////////////////

void __rbTreeDestructorHelper(__Node *nodePtr)
{
    if (nodePtr != &__NIL)
    {
        __rbTreeDestructorHelper(nodePtr->__left);
        __rbTreeDestructorHelper(nodePtr->__right);

        free(nodePtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// RBTree Destructor
////////////////////////////////////////////////////////////////////////////////

void rbTreeDestructor(RBTree *this)
{
    __rbTreeDestructorHelper(this->__root);
}


////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

int main()
{
    RBTree rbTreeObj;

    rbTreeConstructor(&rbTreeObj);

    rbTreeInsert(&rbTreeObj, 0, 0);
    rbTreeInsert(&rbTreeObj, 1, 1);
    rbTreeInsert(&rbTreeObj, 2, 2);
    rbTreeInsert(&rbTreeObj, 3, 3);
    rbTreeInsert(&rbTreeObj, 4, 4);
    rbTreeInsert(&rbTreeObj, 5, 5);
    rbTreeInsert(&rbTreeObj, 6, 6);
    rbTreeInsert(&rbTreeObj, 7, 7);
    rbTreeInsert(&rbTreeObj, 8, 8);
    rbTreeInsert(&rbTreeObj, 9, 9);

    *rbTreeFind(&rbTreeObj, 6) = 666;

    rbTreeErase(&rbTreeObj, 3);
    rbTreeErase(&rbTreeObj, 5);
    rbTreeErase(&rbTreeObj, 4);

    printf("%d\n", *rbTreeFind(&rbTreeObj, 6));

    rbTreeDestructor(&rbTreeObj);
}
