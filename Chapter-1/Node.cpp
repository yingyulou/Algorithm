#include <iostream>
#include <string>

using namespace std;

template <typename Item>
class __Node
{
public:

    Item item;
    __Node *prev, *next;

    __Node(): item(), prev(nullptr), next(nullptr) {}

    __Node(Item __item, __Node *__prev = nullptr, __Node *__next = nullptr):
        item(__item), prev(__prev), next(__next) {}
};


template <typename Item>
class Bag
{
public:

    Bag(): __size(0) { __begin = __end = new __Node<Item>(); }

    Bag<Item> &push(Item item)
    {
        __Node<Item> *newNode = new __Node<Item>(item, __end);
        __end->next = newNode;
        __end = newNode;
        __size++;

        return *this;
    }

    bool isEmpty() { return __size == 0; }

    size_t size() { return __size; }

    __Node<Item> *head() { return __begin->next; }

    ~Bag()
    {
        __Node<Item> *nowNode = __begin, *nextNode;

        while (nowNode)
        {
            nextNode = nowNode->next;
            delete nowNode;
            nowNode = nextNode;
        }
    }


private:

    __Node<Item> *__begin, *__end;
    size_t __size;
};


template <typename Item>
class Stack
{
public:

    Stack(): __size(0) { __begin = __end = new __Node<Item>(); }

    Stack<Item> &push(Item item)
    {
        __Node<Item> *newNode = new __Node<Item>(item, __end);
        __end->next = newNode;
        __end = newNode;
        __size++;

        return *this;
    }

    Stack<Item> &pop()
    {
        __end = __end->prev;
        delete __end->next;
        __end->next = nullptr;
        __size--;

        return *this;
    }

    bool isEmpty() { return __size == 0; }

    size_t size() { return __size; }

    __Node<Item> *head() { return __begin->next; }

    ~Stack()
    {
        __Node<Item> *nowNode = __begin, *nextNode;

        while (nowNode)
        {
            nextNode = nowNode->next;
            delete nowNode;
            nowNode = nextNode;
        }
    }


private:

    __Node<Item> *__begin, *__end;
    size_t __size;
};


template <typename Item>
class Queue
{
public:

    Queue(): __size(0) { __begin = __end = new __Node<Item>(); }

    Queue<Item> &push(Item item)
    {
        __Node<Item> *newNode = new __Node<Item>(item, __end);
        __end->next = newNode;
        __end = newNode;
        __size++;

        return *this;
    }

    Queue<Item> &pop()
    {
        __begin = __begin->next;
        delete __begin->prev;
        __begin->prev = nullptr;
        __size--;

        return *this;
    }

    bool isEmpty() { return __size == 0; }

    size_t size() { return __size; }

    __Node<Item> *head() { return __begin->next; }

    ~Queue()
    {
        __Node<Item> *nowNode = __begin, *nextNode;

        while (nowNode)
        {
            nextNode = nowNode->next;
            delete nowNode;
            nowNode = nextNode;
        }
    }


private:

    __Node<Item> *__begin, *__end;
    size_t __size;
};


int main()
{
    Bag<string> bag;  // Or Stack, Queue

    bag.push("to").push("be").push("or");

    for (auto i = bag.head(); i != nullptr; i = i->next)
    {
        cout << i->item << endl;
    }
}