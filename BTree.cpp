#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

template <typename K, typename V>
struct Node
{
    int N;
    K *key;
    V *value;
    Node<K, V> **sub;
    bool leaf;

    Node(int nodeSize, int __N = 0, bool isLeaf = true): N(__N),
        key(new K[nodeSize]), value(new V[nodeSize]),
        sub(isLeaf ? nullptr : new Node<K, V> *[nodeSize + 1]), leaf(isLeaf) {}


    ~Node()
    {
        delete[] key;
        delete[] value;
        delete[] sub;
    }
};


template <typename K, typename V>
class BTree
{
public:

    BTree(int T = 2): __rootNode(nullptr), __T(T > 2 ? T : 2), __LT(__T - 1),
        __RT(2 * __T - 1) {}


    V &operator[](const K &key)
    {
        if (!__rootNode)
        {
            __rootNode = new Node<K, V>(__RT, 1);
            __rootNode->key[0] = key;

            return __rootNode->value[0];
        }
        else if (__rootNode->N == __RT) __splitRoot();

        Node<K, V> *ownerNode = __rootNode;
        int ownerIdx;

        while (true)
        {
            for (ownerIdx = 0; ownerIdx < ownerNode->N; ownerIdx++)
            {
                if (key < ownerNode->key[ownerIdx]) break;
                else if (key == ownerNode->key[ownerIdx]) return ownerNode->value[ownerIdx];
            }

            if (ownerNode->leaf) break;
            else if (ownerNode->sub[ownerIdx]->N == __RT)
            {
                __splitNode(ownerNode, ownerIdx);

                if (key == ownerNode->key[ownerIdx]) return ownerNode->value[ownerIdx];
                else if (key > ownerNode->key[ownerIdx]) ownerIdx++;
            }

            ownerNode = ownerNode->sub[ownerIdx];
        }

        memmove(ownerNode->key + ownerIdx + 1, ownerNode->key + ownerIdx,
            sizeof(K) * (ownerNode->N - ownerIdx));

        memmove(ownerNode->value + ownerIdx + 1, ownerNode->value + ownerIdx,
            sizeof(V) * (ownerNode->N - ownerIdx));

        ownerNode->N++;
        ownerNode->key[ownerIdx] = key;

        return ownerNode->value[ownerIdx];
    }


    void erase(const K &key)
    {
        if (!__rootNode) return;

        Node<K, V> *ownerNode = __rootNode;
        int ownerIdx;
        bool findBool = false;

        while (!findBool)
        {
            for (ownerIdx = 0; ownerIdx < ownerNode->N; ownerIdx++)
            {
                if (key < ownerNode->key[ownerIdx]) break;
                else if (key == ownerNode->key[ownerIdx])
                {
                    findBool = true;
                    break;
                }
            }

            if (findBool) break;
            else if (ownerNode->leaf) return;
            else if (ownerNode->sub[ownerIdx]->N < __T)
            {
                __repairSearchSub(ownerNode, ownerIdx);
            }

            ownerNode = ownerNode->sub[ownerIdx];
        }

        if (ownerNode->leaf)
        {
            ownerNode->N--;

            memmove(ownerNode->key + ownerIdx, ownerNode->key + ownerIdx + 1,
                sizeof(K) * (ownerNode->N - ownerIdx));

            memmove(ownerNode->value + ownerIdx, ownerNode->value + ownerIdx + 1,
                sizeof(V) * (ownerNode->N - ownerIdx));
        }
        else
        {
            Node<K, V> *nextNode;
            int nextIdx;

            __findNext(ownerNode, ownerIdx, nextNode, nextIdx);

            ownerNode->key[ownerIdx] = nextNode->key[nextIdx];
            ownerNode->value[ownerIdx] = nextNode->value[nextIdx];

            nextNode->N--;

            memmove(nextNode->key + nextIdx, nextNode->key + nextIdx + 1,
                sizeof(K) * (nextNode->N - nextIdx));

            memmove(nextNode->value + nextIdx, nextNode->value + nextIdx + 1,
                sizeof(V) * (nextNode->N - nextIdx));
        }
    }


    ~BTree()
    {
        if (!__rootNode) return;

        queue<Node<K, V> *> nodeQueue;
        nodeQueue.push(__rootNode);
        Node<K, V> *nowNode;

        while (!nodeQueue.empty())
        {
            nowNode = nodeQueue.front();
            nodeQueue.pop();

            if (!nowNode->leaf)
            {
                for (int idx = 0; idx < nowNode->N + 1; idx++)
                {
                    nodeQueue.push(nowNode->sub[idx]);
                }
            }

            delete nowNode;
        }
    }


//private:

    Node<K, V> *__rootNode;
    int __T, __LT, __RT;

    void __splitRoot()
    {
        Node<K, V> *newRootNode = new Node<K, V>(__RT, 1, false),
            *rightNode = new Node<K, V>(__RT, __LT, __rootNode->leaf);

        newRootNode->key[0] = __rootNode->key[__LT];
        newRootNode->sub[0] = __rootNode;
        newRootNode->sub[1] = rightNode;

        __rootNode->N = __LT;

        memcpy(rightNode->key, __rootNode->key + __T, sizeof(K) * __LT);
        memcpy(rightNode->value, __rootNode->value + __T, sizeof(V) * __LT);

        if (!__rootNode->leaf)
        {
            memcpy(rightNode->sub, __rootNode->sub + __T, sizeof(void *) * (__LT + 1));
        }

        __rootNode = newRootNode;
    }


    void __splitNode(Node<K, V> *ownerNode, int ownerIdx)
    {
        Node<K, V> *rightNode = new Node<K, V>(__RT, __LT, ownerNode->sub[ownerIdx]->leaf);

        memmove(ownerNode->key + ownerIdx + 1, ownerNode->key + ownerIdx,
            sizeof(K) * (ownerNode->N - ownerIdx));

        memmove(ownerNode->value + ownerIdx + 1, ownerNode->value + ownerIdx,
            sizeof(V) * (ownerNode->N - ownerIdx));

        memmove(ownerNode->sub + ownerIdx + 2, ownerNode->sub + ownerIdx + 1,
            sizeof(void *) * (ownerNode->N - ownerIdx));

        ownerNode->N++;
        ownerNode->key[ownerIdx] = ownerNode->sub[ownerIdx]->key[__LT];
        ownerNode->value[ownerIdx] = ownerNode->sub[ownerIdx]->value[__LT];
        ownerNode->sub[ownerIdx + 1] = rightNode;

        ownerNode->sub[ownerIdx]->N = __LT;

        memcpy(rightNode->key, ownerNode->sub[ownerIdx]->key + __T,
            sizeof(K) * (__LT));

        memcpy(rightNode->value, ownerNode->sub[ownerIdx]->value + __T,
            sizeof(V) * (__LT));

        if (!ownerNode->sub[ownerIdx]->leaf)
        {
            memcpy(rightNode->sub, ownerNode->sub[ownerIdx]->sub + __T,
                sizeof(void *) * (__LT + 1));
        }
    }


    void __borrowFromLeft(Node<K, V> *ownerNode, int ownerIdx)
    {
        Node<K, V> *leftNode = ownerNode->sub[ownerIdx - 1],
            *subNode = ownerNode->sub[ownerIdx];

        memmove(subNode->key + 1, subNode->key, sizeof(K) * (subNode->N));
        memmove(subNode->value + 1, subNode->value, sizeof(V) * (subNode->N));

        subNode->key[0] = ownerNode->key[ownerIdx - 1];
        subNode->value[0] = ownerNode->value[ownerIdx - 1];

        if (!subNode->leaf)
        {
            memmove(subNode->sub + 1, subNode->sub, sizeof(void *) * (subNode->N + 1));
            subNode->sub[0] = leftNode->sub[leftNode->N];
        }

        ownerNode->key[ownerIdx - 1] = leftNode->key[leftNode->N - 1];
        ownerNode->value[ownerIdx - 1] = leftNode->value[leftNode->N - 1];

        leftNode->N--;
        subNode->N++;
    }


    void __borrowFromRight(Node<K, V> *ownerNode, int ownerIdx)
    {
        Node<K, V> *rightNode = ownerNode->sub[ownerIdx + 1],
            *subNode = ownerNode->sub[ownerIdx];

        subNode->key[subNode->N] = ownerNode->key[ownerIdx];
        subNode->value[subNode->N] = ownerNode->value[ownerIdx];

        ownerNode->key[ownerIdx] = rightNode->key[0];
        ownerNode->value[ownerIdx] = rightNode->value[0];

        memmove(rightNode->key, rightNode->key + 1, sizeof(K) * (rightNode->N));
        memmove(rightNode->value, rightNode->value + 1, sizeof(V) * (rightNode->N));

        if (!rightNode->leaf)
        {
            subNode->sub[subNode->N + 1] = rightNode->sub[0];
            memmove(rightNode->sub, rightNode->sub + 1, sizeof(void *) * (rightNode->N + 1));
        }

        rightNode->N--;
        subNode->N++;
    }


    void __mergeFromLeft(Node<K, V> *ownerNode, int ownerIdx)
    {
    }


    void __mergeFromRight(Node<K, V> *ownerNode, int ownerIdx)
    {
    }


    void __repairSearchSub(Node<K, V> *ownerNode, int ownerIdx)
    {
        if (ownerIdx == 0)
        {
        }
        else if (ownerIdx == ownerNode->N - 1)
        {
        }
        else
        {
        }
    }


    void __findNext(Node<K, V> *ownerNode, int ownerIdx, Node<K, V> *&nextNode,
        int &nextIdx)
    {
    }


    void __repairFindSub(Node<K, V> *ownerNode, int ownerIdx)
    {
    }


    template <typename __K, typename __V>
    friend ostream &operator<<(ostream &os, const BTree<__K, __V> &btree);
};


template <typename K, typename V>
ostream &operator<<(ostream &os, const BTree<K, V> &btree)
{
    if (!btree.__rootNode)
    {
        os << "[]" << endl;
        return os;
    }

    queue<Node<K, V> *> nowQueue, nextQueue;
    nowQueue.push(btree.__rootNode);
    Node<K, V> *nowNode;

    while (true)
    {
        while (!nowQueue.empty())
        {
            nowNode = nowQueue.front();
            nowQueue.pop();

            os << "[";

            for (int idx = 0; idx < nowNode->N; idx++)
            {
                os << " " << nowNode->key[idx] << " ";
            }

            os << "] ";

            if (!nowNode->leaf)
            {
                for (int idx = 0; idx < nowNode->N + 1; idx++)
                {
                    nextQueue.push(nowNode->sub[idx]);
                }
            }
        }

        os << endl;

        if (nowNode->leaf) break;
        else
        {
            nowQueue = nextQueue;
            nextQueue = queue<Node<K, V> *>();
        }
    }

    return os;
}


int main()
{
    BTree<int, int> btree;
    for (int i = 1; i <= 10; i++) btree[i] = i;
    cout << btree << endl;
    btree.__borrowFromRight(btree.__rootNode->sub[1], 1);
    cout << btree << endl;
    btree.__borrowFromLeft(btree.__rootNode->sub[1], 2);
    cout << btree << endl;
}