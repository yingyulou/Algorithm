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
        else if (!nowNode->right) __moveTree(nowNode->left, nowNode);
        else if (!nowNode->left) __moveTree(nowNode->right, nowNode);
        else
        {
            Node<K, V> *nextNode = __findNext(nowNode);

            if (nextNode == nowNode->right)
            {
                __moveTree(nextNode, nowNode);
                nextNode->left = nowNode->left;
                nowNode->left->owner = nextNode;
            }
            else
            {
                __moveTree(nextNode->right, nextNode);
                __moveTree(nextNode, nowNode);
                nextNode->left = nowNode->left;
                nowNode->left->owner = nextNode;
                nextNode->right = nowNode->right;
                nowNode->right->owner = nextNode;
            }
        }

        delete nowNode;
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


    Node<K, V> *pre(Node<K, V> *nowNode)
    {
        if (nowNode->left)
        {
            nowNode = nowNode->left;
            while (nowNode->right) nowNode = nowNode->right;
        }
        else
        {
            while (nowNode->owner && nowNode == nowNode->owner->left)
            {
                nowNode = nowNode->owner;
            }

            nowNode = nowNode->owner;
        }

        return nowNode;
    }


    Node<K, V> *next(Node<K, V> *nowNode)
    {
        if (nowNode->right)
        {
            nowNode = nowNode->right;
            while (nowNode->left) nowNode = nowNode->left;
        }
        else
        {
            while (nowNode->owner && nowNode == nowNode->owner->right)
            {
                nowNode = nowNode->owner;
            }

            nowNode = nowNode->owner;
        }

        return nowNode;
    }


    ~BST()
    {
        if (!__rootNode) return;

        queue<Node<K, V> *> deleteNodeQueue;
        deleteNodeQueue.push(__rootNode);
        Node<K, V> *nowNode;

        while (!deleteNodeQueue.empty())
        {
            nowNode = deleteNodeQueue.front();
            deleteNodeQueue.pop();
            if (nowNode->left)  deleteNodeQueue.push(nowNode->left);
            if (nowNode->right) deleteNodeQueue.push(nowNode->right);
            delete nowNode;
        }
    }


private:

    Node<K, V> *__rootNode = nullptr;

    void __moveTree(Node<K, V> *srcNode, Node<K, V> *tarNode)
    {
        if (tarNode == __rootNode) __rootNode = srcNode;
        else if (tarNode == tarNode->owner->left) tarNode->owner->left = srcNode;
        else tarNode->owner->right = srcNode;

        if (srcNode) srcNode->owner = tarNode->owner;
    }


    Node<K, V> *__findNext(Node<K, V> *nowNode)
    {
        for (nowNode = nowNode->right; nowNode->left; nowNode = nowNode->left);

        return nowNode;
    }
};


int main()
{
    BST<int, int> bst;
    for (int i: {50, 40, 60, 35, 45, 55, 65, 66}) bst[i] = i;
    cout << bst.next(bst.Root()->left->right)->value;
}