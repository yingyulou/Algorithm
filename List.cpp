#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

template <typename T>
struct Node
{
    T data;
    Node<T> *pre, *next;
};


template <typename T>
class List
{
private:

    Node<T> __headNode, *__head, *__last;

public:

    List(): __headNode(), __head(&__headNode), __last(&__headNode) {}

    Node<T> *Head() { return __head->next; }
    Node<T> *Last() { return __last; }

    Node<T> *InsertAfter(Node<T> *insertNode, T insertData)
    {
        Node<T> *newNode = new Node<T> {insertData, insertNode, insertNode->next};
        insertNode->next = newNode;

        if (newNode->next == nullptr)
        {
            __last = newNode;
        }

        return newNode;
    }

    ~List()
    {
        Node<T> *nextNode, *nowNode = __head->next;

        while (nowNode != nullptr)
        {
            nextNode = nowNode->next;
            delete nowNode;
            nowNode = nextNode;
        }
    }
};


int main()
{
    auto v = new List<int>;

    v->InsertAfter(v->Last(), 1);
    v->InsertAfter(v->Last(), 2);
    v->InsertAfter(v->Last(), 3);

    for (auto i = v->Head(); i != nullptr; i = i->next)
    {
        cout << i->data << endl;
    }

    delete v;
}