#include <iostream>
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
    COLOR color;
    Node<K, V> *owner, *left, *right;
};


template <typename K, typename V>
class RBTree
{
public:

    V &operator[](const K &key)
    {
        Node<K, V> *ownerNode = __NIL, *nowNode = __rootNode;

        while (nowNode != __NIL)
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

        if (ownerNode == __NIL)
        {
            __rootNode = new Node<K, V> {key, V(), COLOR::BLACK, __NIL, __NIL, __NIL};

            return __rootNode->value;
        }
        else if (key < ownerNode->key)
        {
            ownerNode->left = new Node<K, V> {key, V(), COLOR::RED, ownerNode, __NIL, __NIL};
            V *valuePtr = &ownerNode->left->value;
            __repairInsert(ownerNode->left);

            return *valuePtr;
        }
        else
        {
            ownerNode->right = new Node<K, V> {key, V(), COLOR::RED, ownerNode, __NIL, __NIL};
            V *valuePtr = &ownerNode->right->value;
            __repairInsert(ownerNode->right);

            return *valuePtr;
        }
    }


    bool find(const K &key)
    {
        Node<K, V> *nowNode = __rootNode;

        while (nowNode != __NIL)
        {
            if (key < nowNode->key) nowNode = nowNode->left;
            else if (key > nowNode->key) nowNode = nowNode->right;
            else return true;
        }

        return false;
    }


    void erase(const K &key)
    {
        Node<K, V> *deleteNode = __rootNode, *moveNode, *preNode;

        while (deleteNode != __NIL)
        {
            if (key < deleteNode->key) deleteNode = deleteNode->left;
            else if (key > deleteNode->key) deleteNode = deleteNode->right;
            else break;
        }

        if (deleteNode == __NIL) return;
        else if (deleteNode->left == __NIL) moveNode = deleteNode->right;
        else if (deleteNode->right == __NIL) moveNode = deleteNode->left;
        else
        {
            preNode = deleteNode->left;
            while (preNode->right != __NIL) preNode = preNode->right;
            deleteNode->key = preNode->key;
            deleteNode->value = preNode->value;
            deleteNode = preNode;
            moveNode = deleteNode->left;
        }

        __moveTree(moveNode, deleteNode);

        if (deleteNode->color == COLOR::BLACK) __repairDelete(moveNode);

        delete deleteNode;
    }


    ~RBTree()
    {
        if (__rootNode == __NIL) return;

        queue<Node<K, V> *> nodeQueue;
        nodeQueue.push(__rootNode);
        Node<K, V> *nowNode;

        while (!nodeQueue.empty())
        {
            nowNode = nodeQueue.front();
            nodeQueue.pop();
            if (nowNode->left != __NIL) nodeQueue.push(nowNode->left);
            if (nowNode->right != __NIL) nodeQueue.push(nowNode->right);
            delete nowNode;
        }
    }


private:

    Node<K, V> __nilNode {K(), V(), COLOR::BLACK, nullptr, nullptr, nullptr},
        *__NIL = &__nilNode, *__rootNode = __NIL;

    void __rotateLeft(Node<K, V> *rotateNode)
    {
        Node<K, V> *rightNode = rotateNode->right;

        rightNode->owner = rotateNode->owner;

        if (rotateNode->owner == __NIL) __rootNode = rightNode;
        else if (rotateNode == rotateNode->owner->left) rotateNode->owner->left = rightNode;
        else rotateNode->owner->right = rightNode;

        rotateNode->right = rightNode->left;
        if (rightNode->left != __NIL) rightNode->left->owner = rotateNode;

        rightNode->left = rotateNode;
        rotateNode->owner = rightNode;
    }


    void __rotateRight(Node<K, V> *rotateNode)
    {
        Node<K, V> *leftNode = rotateNode->left;

        leftNode->owner = rotateNode->owner;

        if (rotateNode->owner == __NIL) __rootNode = leftNode;
        else if (rotateNode == rotateNode->owner->left) rotateNode->owner->left = leftNode;
        else rotateNode->owner->right = leftNode;

        rotateNode->left = leftNode->right;
        if (leftNode->right != __NIL) leftNode->right->owner = rotateNode;

        leftNode->right = rotateNode;
        rotateNode->owner = leftNode;
    }


    void __repairInsert(Node<K, V> *insertNode)
    {
        Node<K, V> *brotherNode;

        while (insertNode->owner->color == COLOR::RED)
        {
            if (insertNode->owner == insertNode->owner->owner->left)
            {
                brotherNode = insertNode->owner->owner->right;

                if (brotherNode->color == COLOR::RED)
                {
                    insertNode->owner->color = brotherNode->color = COLOR::BLACK;
                    insertNode->owner->owner->color = COLOR::RED;
                    insertNode = insertNode->owner->owner;
                }
                else
                {
                    if (insertNode == insertNode->owner->right)
                    {
                        insertNode = insertNode->owner;
                        __rotateLeft(insertNode);
                    }

                    insertNode->owner->color = COLOR::BLACK;
                    insertNode->owner->owner->color = COLOR::RED;
                    __rotateRight(insertNode->owner->owner);
                }
            }
            else
            {
                brotherNode = insertNode->owner->owner->left;

                if (brotherNode->color == COLOR::RED)
                {
                    insertNode->owner->color = brotherNode->color = COLOR::BLACK;
                    insertNode->owner->owner->color = COLOR::RED;
                    insertNode = insertNode->owner->owner;
                }
                else
                {
                    if (insertNode == insertNode->owner->left)
                    {
                        insertNode = insertNode->owner;
                        __rotateRight(insertNode);
                    }

                    insertNode->owner->color = COLOR::BLACK;
                    insertNode->owner->owner->color = COLOR::RED;
                    __rotateLeft(insertNode->owner->owner);
                }
            }
        }

        __rootNode->color = COLOR::BLACK;
    }


    void __moveTree(Node<K, V> *srcNode, Node<K, V> *tarNode)
    {
        srcNode->owner = tarNode->owner;

        if (tarNode->owner == __NIL) __rootNode = srcNode;
        else if (tarNode == tarNode->owner->left) tarNode->owner->left = srcNode;
        else tarNode->owner->right = srcNode;
    }


    void __repairDelete(Node<K, V> *moveNode)
    {
        Node<K, V> *brotherNode;

        while (moveNode != __rootNode && moveNode->color == COLOR::BLACK)
        {
            if (moveNode == moveNode->owner->left)
            {
                brotherNode = moveNode->owner->right;

                if (brotherNode->color == COLOR::RED)
                {
                    brotherNode->color = COLOR::BLACK;
                    brotherNode->owner->color = COLOR::RED;
                    __rotateLeft(brotherNode->owner);
                    brotherNode = moveNode->owner->right;
                }

                if (brotherNode->left->color == COLOR::BLACK &&
                    brotherNode->right->color == COLOR::BLACK)
                {
                    brotherNode->color = COLOR::RED;
                    moveNode = moveNode->owner;
                }
                else
                {
                    if (brotherNode->right->color == COLOR::BLACK)
                    {
                        brotherNode->color = COLOR::RED;
                        brotherNode->left->color = COLOR::BLACK;
                        __rotateRight(brotherNode);
                        brotherNode = moveNode->owner->right;
                    }

                    brotherNode->color = brotherNode->owner->color;
                    brotherNode->owner->color = brotherNode->right->color = COLOR::BLACK;
                    __rotateLeft(brotherNode->owner);
                    moveNode = __rootNode;
                }
            }
            else
            {
                brotherNode = moveNode->owner->left;

                if (brotherNode->color == COLOR::RED)
                {
                    brotherNode->color = COLOR::BLACK;
                    brotherNode->owner->color = COLOR::RED;
                    __rotateRight(brotherNode->owner);
                    brotherNode = moveNode->owner->left;
                }

                if (brotherNode->left->color == COLOR::BLACK &&
                    brotherNode->right->color == COLOR::BLACK)
                {
                    brotherNode->color = COLOR::RED;
                    moveNode = moveNode->owner;
                }
                else
                {
                    if (brotherNode->left->color == COLOR::BLACK)
                    {
                        brotherNode->color = COLOR::RED;
                        brotherNode->right->color = COLOR::BLACK;
                        __rotateLeft(brotherNode);
                        brotherNode = moveNode->owner->left;
                    }

                    brotherNode->color = brotherNode->owner->color;
                    brotherNode->left->color = brotherNode->owner->color = COLOR::BLACK;
                    __rotateRight(brotherNode->owner);
                    moveNode = __rootNode;
                }
            }
        }

        moveNode->color = COLOR::BLACK;
    }


template<typename __K, typename __V>
friend ostream &operator<<(ostream &os, const RBTree<__K, __V> &rbtree);
};


template<typename K, typename V>
ostream &operator<<(ostream &os, const RBTree<K, V> &rbtree)
{
    vector<Node<K, V> *> nowNodeList {rbtree.__rootNode}, nextNodeList;

    while (!nowNodeList.empty())
    {
        for (auto nowNode: nowNodeList)
        {
            if (nowNode == rbtree.__NIL)
            {
                os << "NIL ";
            }
            else
            {
                os << nowNode->key << '-' << (nowNode->color == COLOR::RED ? 'R' : 'B') << ' ';
                nextNodeList.push_back(nowNode->left);
                nextNodeList.push_back(nowNode->right);
            }
        }

        cout << endl;

        nowNodeList = nextNodeList;
        nextNodeList.clear();
    }

    return os;
}


int main()
{
    RBTree<int, int> rbtree;
    for (int i = 0; i < 10; i++) rbtree[i] = i;
    rbtree.erase(3);
    rbtree.erase(5);
    rbtree.erase(4);
    rbtree.erase(2);
    rbtree.erase(1);
    rbtree.erase(7);
    rbtree.erase(6);
    rbtree.erase(8);
    rbtree.erase(0);
    rbtree.erase(9);
    cout << rbtree;
}