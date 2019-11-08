#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename V>
struct Node
{
    V value = V();
    Node<V> *next = nullptr;

    ~Node() { delete[] next; }
};


template <typename V>
class Trie
{
public:

    V &operator[](const string &key)
    {
        Node<V> *nowNode = headNode;

        for (int idx = 0; idx < key.size(); idx++)
        {
            if (!nowNode->next) nowNode->next = new Node<V> [ALPHABET_COUNT];
            nowNode = nowNode->next + key[idx];
        }

        return nowNode->value;
    }


    ~Trie() { delete headNode; }


private:

    static const int ALPHABET_COUNT = 256;
    Node<V> *headNode = new Node<V>;
};


int main()
{
    Trie<string> v;
    vector<string> keyList {"a", "bc", "def", "ghij", "kloum", "haha", "vczzvbdsfsdf"};

    for (auto &s: keyList)
    {
        v[s] = s;
    }

    for (auto &s: keyList)
    {
        cout << v[s] << endl;
    }
}