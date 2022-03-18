#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

////////////////////////////////////////////////////////////////////////////////
// Struct __Node
////////////////////////////////////////////////////////////////////////////////

typedef struct __Node
{
    int __key;
    int __value;
    struct __Node *__owner;
    struct __Node *__left;
    struct __Node *__right;
    size_t __height;
} __Node;


////////////////////////////////////////////////////////////////////////////////
// Nil Node
////////////////////////////////////////////////////////////////////////////////

__Node __NIL = {};


////////////////////////////////////////////////////////////////////////////////
// __Node Constructor
////////////////////////////////////////////////////////////////////////////////

void __nodeConstructor(__Node *this, int key, int value, __Node *owner)
{
    this->__key    = key;
    this->__value  = value;
    this->__owner  = owner;
    this->__left   = &__NIL;
    this->__right  = &__NIL;
    this->__height = 1;
}


////////////////////////////////////////////////////////////////////////////////
// Struct AVLTree
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    __Node *__root;
} AVLTree;


////////////////////////////////////////////////////////////////////////////////
// AVLTree Constructor
////////////////////////////////////////////////////////////////////////////////

void avlTreeConstructor(AVLTree *this)
{
    this->__root = &__NIL;
}


////////////////////////////////////////////////////////////////////////////////
// AVLTree Find
////////////////////////////////////////////////////////////////////////////////

int *avlTreeFind(AVLTree *this, int key)
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
// Repair Height
////////////////////////////////////////////////////////////////////////////////

void __repairHeight(__Node *nodePtr)
{
    if (nodePtr->__left->__height > nodePtr->__right->__height)
    {
        nodePtr->__height = nodePtr->__left->__height + 1;
    }
    else
    {
        nodePtr->__height = nodePtr->__right->__height + 1;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Rotate Left
////////////////////////////////////////////////////////////////////////////////

void __rotateLeft(AVLTree *this, __Node *nodePtr)
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

    __repairHeight(nodePtr);
    __repairHeight(rightNodePtr);
}


////////////////////////////////////////////////////////////////////////////////
// Rotate Right
////////////////////////////////////////////////////////////////////////////////

void __rotateRight(AVLTree *this, __Node *nodePtr)
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

    __repairHeight(nodePtr);
    __repairHeight(leftNodePtr);
}


////////////////////////////////////////////////////////////////////////////////
// AVLTree Repair
////////////////////////////////////////////////////////////////////////////////

void __avlTreeRepair(AVLTree *this, __Node *nodePtr)
{
    for (; nodePtr != &__NIL; nodePtr = nodePtr->__owner)
    {
        if (nodePtr->__left->__height > nodePtr->__right->__height)
        {
            nodePtr->__height = nodePtr->__left->__height + 1;

            if (nodePtr->__left->__height - nodePtr->__right->__height > 1)
            {
                if (nodePtr->__left->__right->__height > nodePtr->__left->__left->__height)
                {
                    __rotateLeft(this, nodePtr->__left);
                }

                __rotateRight(this, nodePtr);
            }
        }
        else
        {
            nodePtr->__height = nodePtr->__right->__height + 1;

            if (nodePtr->__right->__height - nodePtr->__left->__height > 1)
            {
                if (nodePtr->__right->__left->__height > nodePtr->__right->__right->__height)
                {
                    __rotateRight(this, nodePtr->__right);
                }

                __rotateLeft(this, nodePtr);
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// AVLTree Insert
////////////////////////////////////////////////////////////////////////////////

void avlTreeInsert(AVLTree *this, int key, int value)
{
    if (this->__root == &__NIL)
    {
        this->__root = (__Node *)malloc(sizeof(__Node));

        __nodeConstructor(this->__root, key, value, &__NIL);
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

                    __avlTreeRepair(this, nodePtr);

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

                    __avlTreeRepair(this, nodePtr);

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

__Node *__findDeleteNode(AVLTree *this, int key)
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
// AVLTree Erase
////////////////////////////////////////////////////////////////////////////////

void avlTreeErase(AVLTree *this, int key)
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
        else
        {
            if (nodePtr == nodePtr->__owner->__left)
            {
                nodePtr->__owner->__left = replaceNodePtr;
            }
            else
            {
                nodePtr->__owner->__right = replaceNodePtr;
            }

            __avlTreeRepair(this, nodePtr->__owner);
        }

        free(nodePtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// AVLTree Destructor Helper
////////////////////////////////////////////////////////////////////////////////

void __avlTreeDestructorHelper(__Node *nodePtr)
{
    if (nodePtr != &__NIL)
    {
        __avlTreeDestructorHelper(nodePtr->__left);
        __avlTreeDestructorHelper(nodePtr->__right);

        free(nodePtr);
    }
}


////////////////////////////////////////////////////////////////////////////////
// AVLTree Destructor
////////////////////////////////////////////////////////////////////////////////

void avlTreeDestructor(AVLTree *this)
{
    __avlTreeDestructorHelper(this->__root);
}


////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

int main()
{
    AVLTree avlTreeObj;

    avlTreeConstructor(&avlTreeObj);

    avlTreeInsert(&avlTreeObj, 0, 0);
    avlTreeInsert(&avlTreeObj, 1, 1);
    avlTreeInsert(&avlTreeObj, 2, 2);
    avlTreeInsert(&avlTreeObj, 3, 3);
    avlTreeInsert(&avlTreeObj, 4, 4);
    avlTreeInsert(&avlTreeObj, 5, 5);
    avlTreeInsert(&avlTreeObj, 6, 6);
    avlTreeInsert(&avlTreeObj, 7, 7);
    avlTreeInsert(&avlTreeObj, 8, 8);
    avlTreeInsert(&avlTreeObj, 9, 9);

    *avlTreeFind(&avlTreeObj, 6) = 666;

    avlTreeErase(&avlTreeObj, 4);
    avlTreeErase(&avlTreeObj, 9);
    avlTreeErase(&avlTreeObj, 8);

    printf("%d\n", *avlTreeFind(&avlTreeObj, 6));

    avlTreeDestructor(&avlTreeObj);
}
