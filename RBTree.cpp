#include <iostream>
#include <stack>
#include <queue>

using namespace std;

enum class COLOR
{
    RED,
    BLACK,
};


template <typename K, typename V>
struct Node
{
    K key;
    V value;
    Node<K, V> *left;
    Node<K, V> *right;
    COLOR color;
};


template <typename K, typename V>
class RBTree
{
public:

    RBTree(): headNode(nullptr) {}


    const Node<K, V> *GetHeadNode() const { return headNode; }


    bool find(const K &key)
    {
        Node<K, V> *nowNode = headNode;

        while (nowNode)
        {
            if (key < nowNode->key) nowNode = nowNode->left;
            else if (key > nowNode->key) nowNode = nowNode->right;
            else return true;
        }

        return false;
    }


    V &operator[](const K &key)
    {
        stack<Node<K, V> **> nodeStack;
        nodeStack.push(&headNode);
        Node<K, V> **nowNode;

        for (nowNode = nodeStack.top(); *nowNode; nowNode = nodeStack.top())
        {
            if (key < (*nowNode)->key) nodeStack.push(&(*nowNode)->left);
            else if (key > (*nowNode)->key) nodeStack.push(&(*nowNode)->right);
            else return (*nowNode)->value;
        }

        *nowNode = new Node<K, V> {key, V(), nullptr, nullptr, COLOR::RED};
        V *valuePtr = &(*nowNode)->value;

        while (nodeStack.size())
        {
            nowNode = nodeStack.top();
            nodeStack.pop();
            if (!isRed((*nowNode)->left) && isRed((*nowNode)->right)) rotateLeft(nowNode);
            if (isRed((*nowNode)->left) && isRed((*nowNode)->left->left)) rotateRight(nowNode);
            if (isRed((*nowNode)->left) && isRed((*nowNode)->right)) exchangeColor(nowNode);
        }

        return *valuePtr;
    }


    ~RBTree()
    {
        stack<Node<K, V> *> nodeStack;
        nodeStack.push(headNode);
        Node<K, V> *nowNode;

        while (nodeStack.size())
        {
            nowNode = nodeStack.top();
            nodeStack.pop();
            if (nowNode->left) nodeStack.push(nowNode->left);
            if (nowNode->right) nodeStack.push(nowNode->right);
            delete nowNode;
        }
    }


private:

    Node<K, V> *headNode;


    static bool isRed(Node<K, V> *nodePtr)
    {
        return nodePtr && nodePtr->color == COLOR::RED;
    }


    static void rotateLeft(Node<K, V> **ownerNode)
    {
        Node<K, V> *tmpNode = (*ownerNode)->right;
        (*ownerNode)->right = tmpNode->left;
        tmpNode->left = *ownerNode;
        tmpNode->color = (*ownerNode)->color;
        (*ownerNode)->color = COLOR::RED;
        *ownerNode = tmpNode;
    }


    static void rotateRight(Node<K, V> **ownerNode)
    {
        Node<K, V> *tmpNode = (*ownerNode)->left;
        (*ownerNode)->left = tmpNode->right;
        tmpNode->right = *ownerNode;
        tmpNode->color = (*ownerNode)->color;
        (*ownerNode)->color = COLOR::RED;
        *ownerNode = tmpNode;
    }


    static void exchangeColor(Node<K, V> **ownerNode)
    {
        (*ownerNode)->left->color = (*ownerNode)->right->color = COLOR::BLACK;
        (*ownerNode)->color = COLOR::RED;
    }
};


int main()
{
    RBTree<int, double> m;

    for (int i = 1; i <= 10; i++) m[i] = i;
    for (int i = 1; i < 10; i++) cout << m[i] << endl;
}