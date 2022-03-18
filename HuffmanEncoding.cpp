#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <pprint/pprint>

////////////////////////////////////////////////////////////////////////////////
// Using
////////////////////////////////////////////////////////////////////////////////

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class __Node
////////////////////////////////////////////////////////////////////////////////

class __Node
{
    // Friend
    friend class HuffmanEncoding;


public:

    // Constructor
    explicit __Node(int freq, char value = 0, __Node *left = nullptr, __Node *right = nullptr):
        __freq (freq),
        __value(value),
        __left (left),
        __right(right) {}


private:

    // Attribute
    int __freq;
    char __value;
    __Node *__left;
    __Node *__right;
};


////////////////////////////////////////////////////////////////////////////////
// Class HuffmanEncoding
////////////////////////////////////////////////////////////////////////////////

class HuffmanEncoding
{
public:

    // Constructor
    explicit HuffmanEncoding(const string &encodingStr):
        __encodingStr(encodingStr)
    {
        __createHuffmanTree();
        __createHuffmanEncodingMap();
    }


    // Encode
    string encode(const string &oriStr) const
    {
        string encodeStr;

        for (auto curChar: oriStr)
        {
            encodeStr += __huffmanEncodingMap.at(curChar);
        }

        return encodeStr;
    }


    // Decode
    string decode(const string &oriStr) const
    {
        string decodeStr;

        auto nodePtr = __huffmanTreeRoot;

        for (auto curChar: oriStr)
        {
            if (curChar == '0')
            {
                nodePtr = nodePtr->__left;
            }
            else
            {
                nodePtr = nodePtr->__right;
            }

            if (nodePtr->__value)
            {
                decodeStr += nodePtr->__value;
                nodePtr = __huffmanTreeRoot;
            }
        }

        return decodeStr;
    }


    // Destructor
    ~HuffmanEncoding()
    {
        __destructorHelper(__huffmanTreeRoot);
    }


private:

    // Attribute
    string __encodingStr;
    __Node *__huffmanTreeRoot;
    unordered_map<char, string> __huffmanEncodingMap;


    // Create Huffman Tree
    void __createHuffmanTree()
    {
        unordered_map<char, int> counterMap {{0, 0}};

        for (auto curChar: __encodingStr)
        {
            counterMap[curChar]++;
        }

        vector<__Node *> nodePtrList;

        for (auto &[curChar, curFreq]: counterMap)
        {
            nodePtrList.push_back(new __Node(curFreq, curChar));
        }

        while (nodePtrList.size() > 1)
        {
            int firstMinIdx, secondMinIdx, firstMinFreq = INT_MAX, secondMinFreq = INT_MAX;

            for (int idx = 0; idx < nodePtrList.size(); idx++)
            {
                if (nodePtrList[idx]->__freq < firstMinFreq)
                {
                    secondMinIdx  = firstMinIdx;
                    secondMinFreq = firstMinFreq;

                    firstMinIdx  = idx;
                    firstMinFreq = nodePtrList[idx]->__freq;
                }
                else if (nodePtrList[idx]->__freq < secondMinFreq)
                {
                    secondMinIdx  = idx;
                    secondMinFreq = nodePtrList[idx]->__freq;
                }
            }

            nodePtrList.push_back(new __Node(firstMinFreq + secondMinFreq, 0,
                nodePtrList[firstMinIdx], nodePtrList[secondMinIdx]));

            swap(nodePtrList[firstMinIdx], nodePtrList.back());
            nodePtrList.pop_back();

            swap(nodePtrList[secondMinIdx], nodePtrList.back());
            nodePtrList.pop_back();
        }

        __huffmanTreeRoot = nodePtrList[0];
    }


    // Create Huffman Encoding Map Helper
    void __createHuffmanEncodingMapHelper(__Node *root, string curEncodingStr)
    {
        if (root)
        {
            if (root->__value)
            {
                __huffmanEncodingMap[root->__value] = curEncodingStr;
            }

            __createHuffmanEncodingMapHelper(root->__left, curEncodingStr + "0");
            __createHuffmanEncodingMapHelper(root->__right, curEncodingStr + "1");
        }
    }


    // Create Huffman Encoding Map
    void __createHuffmanEncodingMap()
    {
        __createHuffmanEncodingMapHelper(__huffmanTreeRoot, "");
    }


    // Destructor Helper
    void __destructorHelper(__Node *root)
    {
        if (root)
        {
            __destructorHelper(root->__left);
            __destructorHelper(root->__right);

            delete root;
        }
    }
};


////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

int main()
{
    HuffmanEncoding huffmanEncoding("abbcccddddeeeeefffffff");

    pprint::print(huffmanEncoding.decode(huffmanEncoding.encode("abcdef")));
}
