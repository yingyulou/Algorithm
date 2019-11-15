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

    Node<K, V> *Root() { return rootNode; }


    V &operator[](const K &key)
    {
        Node<K, V> *ownerNode = nullptr, *nowNode = rootNode;

        while (nowNode)
        {
            if (key < nowNode->key) { ownerNode = nowNode; nowNode = nowNode->left; }
            else if (key > nowNode->key) { ownerNode = nowNode; nowNode = nowNode->right; }
            else return nowNode->value;
        }

        if (!ownerNode)
        {
            rootNode = new Node<K, V> {key, V(), nullptr, nullptr, nullptr};
            return rootNode->value;
        }
        else if (key < ownerNode->key)
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


    Node<K, V> *pre(Node<K, V> *nowNode)
    {
        if (nowNode->left)
        {
            for (nowNode = nowNode->left; nowNode->right; nowNode = nowNode->right);
            return nowNode;
        }

        Node<K, V> *ownerNode = nowNode->owner;

        while (ownerNode && nowNode == ownerNode->left)
        {
            nowNode = ownerNode;
            ownerNode = nowNode->owner;
        }

        return ownerNode;
    }


    Node<K, V> *next(Node<K, V> *nowNode)
    {
        if (nowNode->right)
        {
            for (nowNode = nowNode->right; nowNode->left; nowNode = nowNode->left);
            return nowNode;
        }

        Node<K, V> *ownerNode = nowNode->owner;

        while (ownerNode && nowNode == ownerNode->right)
        {
            nowNode = ownerNode;
            ownerNode = nowNode->owner;
        }

        return ownerNode;
    }


    void erase(const K &key)
    {
        Node<K, V> *oldNode = rootNode, *nextNode;

        while (oldNode)
        {
            if (key < oldNode->key) oldNode = oldNode->left;
            else if (key > oldNode->key) oldNode = oldNode->right;
            else break;
        }

        if (!oldNode) return;
        else if (!oldNode->left) __transplant(oldNode, oldNode->right);
        else if (!oldNode->right) __transplant(oldNode, oldNode->left);
        else
        {
            nextNode = next(oldNode);

            if (nextNode->owner != oldNode)
            {
                __transplant(nextNode, nextNode->right);
                nextNode->right = oldNode->right;
                nextNode->right->owner = nextNode;
            }

            __transplant(oldNode, nextNode);
            nextNode->left = oldNode->left;
            nextNode->left->owner = nextNode;
        }

        delete oldNode;
    }


    ~BST()
    {
        queue<Node<K, V> *> deleteQueue;
        deleteQueue.push(rootNode);
        Node<K, V> *nowNode;

        while (deleteQueue.size())
        {
            nowNode = deleteQueue.front();
            deleteQueue.pop();
            if (nowNode->left) deleteQueue.push(nowNode->left);
            if (nowNode->right) deleteQueue.push(nowNode->right);
            delete nowNode;
        }
    }


private:

    Node<K, V> *rootNode = nullptr;

    void __transplant(Node<K, V> *oldNode, Node<K, V> *newNode)
    {
        if (!oldNode->owner) rootNode = newNode;
        else if (oldNode == oldNode->owner->left) oldNode->owner->left = newNode;
        else oldNode->owner->right = newNode;

        if (newNode) newNode->owner = oldNode->owner;
    }
};


int main()
{
    BST<int, int> bst;
    for (int i: {100, 90, 110, 85, 95, 105, 115}) bst[i] = i;
    bst.erase(100);
}