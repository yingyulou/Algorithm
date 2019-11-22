#include <iostream>
#include <queue>

using namespace std;

template <typename K, typename V>
struct Node
{
    K key;
    V value;
    Node<K, V> *owner, *left, *right;
};


template <typename K, typename V>
class BST
{
public:

    Node<K, V> *Root() { return __rootNode; }


    V &operator[](const K &key)
    {
        Node<K, V> *ownerNode = nullptr, *nowNode = __rootNode;

        while (nowNode)
        {
            if (key < nowNode->key)
            {
                ownerNode = nowNode;
                nowNode = nowNode->left;
            }
            else if (key > nowNode->key)
            {
                ownerNode = nowNode;
                nowNode = nowNode->right;
            }
            else return nowNode->value;
        }

        if (ownerNode)
        {
            if (key < ownerNode->key)
            {
                ownerNode->left = new Node<K, V> {key, V(), ownerNode, nullptr, nullptr};
                return ownerNode->left->value;
            }
            else
            {
                ownerNode->right = new Node<K, V> {key, V(), ownerNode, nullptr, nullptr};
                return ownerNode->right->value;
            }
        }
        else
        {
            __rootNode = new Node<K, V> {key, V(), nullptr, nullptr, nullptr};
            return __rootNode->value;
        }
    }


    bool find(const K &key)
    {
        Node<K, V> *nowNode = __rootNode;

        while (nowNode)
        {
            if (key < nowNode->key) nowNode = nowNode->left;
            else if (key > nowNode->key) nowNode = nowNode->right;
            else return true;
        }

        return false;
    }


    void erase(const K &key)
    {
        Node<K, V> *nowNode = __rootNode;

        while (nowNode)
        {
            if (key < nowNode->key) nowNode = nowNode->left;
            else if (key > nowNode->key) nowNode = nowNode->right;
            else break;
        }

        if (!nowNode) return;
        else if (!nowNode->left)
        {
            __moveTree(nowNode->right, nowNode);
            delete nowNode;
        }
        else if (!nowNode->right)
        {
            __moveTree(nowNode->left, nowNode);
            delete nowNode;
        }
        else
        {
            Node<K, V> *nextNode = nowNode->right;
            while (nextNode->left) nextNode = nextNode->left;
            nowNode->key = nextNode->key;
            nowNode->value = nextNode->value;
            __moveTree(nextNode->right, nextNode);
            delete nextNode;
        }
    }


    ~BST()
    {
        queue<Node<K, V> *> nodeQueue;
        nodeQueue.push(__rootNode);
        Node<K, V> *nowNode;

        while (!nodeQueue.empty())
        {
            nowNode = nodeQueue.front();
            nodeQueue.pop();
            if (nowNode->left) nodeQueue.push(nowNode->left);
            if (nowNode->right) nodeQueue.push(nowNode->right);
            delete nowNode;
        }
    }


private:

    Node<K, V> *__rootNode = nullptr;

    void __moveTree(Node<K, V> *srcNode, Node<K, V> *tarNode)
    {
        if (srcNode) srcNode->owner = tarNode->owner;

        if (tarNode == __rootNode) __rootNode = srcNode;
        else if (tarNode == tarNode->owner->left) tarNode->owner->left = srcNode;
        else tarNode->owner->right = srcNode;
    }
};


int main()
{
    BST<int, int> bst;
    for (int i: {50, 40, 60, 35, 45, 55, 65, 66}) bst[i] = i;
}