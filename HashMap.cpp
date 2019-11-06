#include <iostream>
#include <functional>

using namespace std;

template <typename K, typename V>
struct Node
{
    K key;
    V value;
    Node<K, V> *next = nullptr;
};


template <typename K, typename V>
class HashMap
{
public:

    V &operator[](const K &key)
    {
        if (__size >= __rehashThreshold) __rehash();
        Node<K, V> **nowNode = &__hashMap[hash<K>()(key) % __barrel].next;

        while (*nowNode)
        {
            if ((*nowNode)->key == key) return (*nowNode)->value;
            else nowNode = &(*nowNode)->next;
        }

        *nowNode = new Node<K, V> {key, V(), nullptr};
        __size++;

        return (*nowNode)->value;
    }


    bool find(const K &key) const
    {
        for (Node<K, V> *nowNode = __hashMap[hash<K>()(key) % __barrel].next;
            nowNode; nowNode = nowNode->next)
        {
            if (nowNode->key == key) return true;
        }

        return false;
    }


    HashMap<K, V> &erase(const K &key)
    {
        for (Node<K, V> *preNode = __hashMap + hash<K>()(key) % __barrel;
            preNode; preNode = preNode->next)
        {
            if (preNode->next && preNode->next->key == key)
            {
                Node<K, V> *deleteNode = preNode->next;
                preNode->next = preNode->next->next;
                delete deleteNode;
            }
        }

        return *this;
    }


    ~HashMap() { __deallocateHashMap(); }


private:

    int __size = 0;
    int __barrel = 0;
    double __loadFactor = 0.75;
    int __rehashThreshold = __barrel * __loadFactor;
    Node<K, V> *__hashMap = nullptr;

    void __deallocateHashMap()
    {
        Node<K, V> *nowNode, *nextNode;

        for (int barrelIdx = 0; barrelIdx < __barrel; barrelIdx++)
        {
            nowNode = __hashMap[barrelIdx].next;

            while (nowNode)
            {
                nextNode = nowNode->next;
                delete nowNode;
                nowNode = nextNode;
            }
        }

        delete[] __hashMap;
    }


    void __rehash()
    {
        int newBarrelCount = (__barrel + 1) * 2;
        Node<K, V> *newHashMap = new Node<K, V> [newBarrelCount], *nowNode, *newPreNode;

        for (int barrelIdx = 0; barrelIdx < __barrel; barrelIdx++)
        {
            for (nowNode = __hashMap[barrelIdx].next; nowNode; nowNode = nowNode->next)
            {
                newPreNode = newHashMap + hash<K>()(nowNode->key) % newBarrelCount;
                while (newPreNode->next) newPreNode = newPreNode->next;
                newPreNode->next = new Node<K, V> {nowNode->key, nowNode->value, nullptr};
            }
        }

        __deallocateHashMap();
        __barrel = newBarrelCount;
        __rehashThreshold = __barrel * __loadFactor;
        __hashMap = newHashMap;
    }
};


int main()
{
    HashMap<string, string> m;

    m["ALA"] = "A";
    m["ARG"] = "R";
    m["ASN"] = "N";
    m["ASP"] = "D";
    m["CYS"] = "C";
    m["GLN"] = "Q";
    m["GLU"] = "E";
    m["GLY"] = "G";
    m["HIS"] = "H";
    m["ILE"] = "I";
    m["LEU"] = "L";
    m["LYS"] = "K";
    m["MET"] = "M";
    m["PHE"] = "F";
    m["PRO"] = "P";
    m["SER"] = "S";
    m["THR"] = "T";
    m["TRP"] = "W";
    m["TYR"] = "Y";
    m["VAL"] = "V";

    m.erase("ARG").erase("ASP").erase("GLY").erase("LYS").erase("TRP");

    cout << m.find("ALA") << endl;
    cout << m.find("ARG") << endl;
    cout << m.find("ASN") << endl;
    cout << m.find("ASP") << endl;
    cout << m.find("CYS") << endl;
    cout << m.find("GLN") << endl;
    cout << m.find("GLU") << endl;
    cout << m.find("GLY") << endl;
    cout << m.find("HIS") << endl;
    cout << m.find("ILE") << endl;
    cout << m.find("LEU") << endl;
    cout << m.find("LYS") << endl;
    cout << m.find("MET") << endl;
    cout << m.find("PHE") << endl;
    cout << m.find("PRO") << endl;
    cout << m.find("SER") << endl;
    cout << m.find("THR") << endl;
    cout << m.find("TRP") << endl;
    cout << m.find("TYR") << endl;
    cout << m.find("VAL") << endl;
}