#include <iostream>
#include <vector>
#include <utility>
#include <cassert>
#include <algorithm>
#include <cstddef>
#include <ctime>
#include <cstdlib>

using namespace std;

void __doublePartition(vector<int> &numList, int leftIdx, int rightIdx)
{
    if (leftIdx >= rightIdx)
    {
        return;
    }

    int midIdx = leftIdx;

    for (int nowIdx = leftIdx + 1; nowIdx <= rightIdx; nowIdx++)
    {
        if (numList[nowIdx] < numList[leftIdx])
        {
            swap(numList[nowIdx], numList[++midIdx]);
        }
    }

    swap(numList[leftIdx], numList[midIdx]);

    __doublePartition(numList, leftIdx, midIdx);
    __doublePartition(numList, midIdx + 1, rightIdx);
}


void __triplePartition(vector<int> &numList, int leftIdx, int rightIdx)
{
    if (leftIdx >= rightIdx)
    {
        return;
    }

    int midLeft = leftIdx, midRight = rightIdx, nowIdx = leftIdx + 1;

    while (nowIdx <= midRight)
    {
        if (numList[nowIdx] < numList[midLeft])
        {
            swap(numList[nowIdx++], numList[midLeft++]);
        }
        else if (numList[nowIdx] > numList[midLeft])
        {
            swap(numList[nowIdx], numList[midRight--]);
        }
        else
        {
            nowIdx++;
        }
    }

    __triplePartition(numList, leftIdx, midLeft - 1);
    __triplePartition(numList, midRight + 1, rightIdx);
}


void QuickSort(vector<int> &numList)
{
//    __doublePartition(numList, 0, numList.size() - 1);
    __triplePartition(numList, 0, numList.size() - 1);
}


int main()
{
    srand(time(NULL));
    vector<int> numList;

    for (int i = 0; i < 20; i++)
    {
        numList.push_back(rand() % 100);
    }

    QuickSort(numList);
    assert(is_sorted(numList.begin(), numList.end()));
}
