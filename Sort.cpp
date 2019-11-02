#include <cassert>
#include <iostream>
#include <vector>
#include <cstddef>
#include <ctime>
#include <cstdlib>
#include <utility>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Insertion Sort
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void InsertionSort(vector<T> &numList)
{
    for (int i = 1; i < numList.size(); i++)
    {
        for (int j = i; j > 0; j--)
        {
            if (numList[j] < numList[j - 1])
            {
                swap(numList[j], numList[j - 1]);
            }
            else
            {
                break;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// Selection Sort
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void SelectionSort(vector<T> &numList)
{
    for (int i = 0; i < numList.size() - 1; i++)
    {
        int minIdx = i;

        for (int j = i + 1; j < numList.size(); j++)
        {
            if (numList[j] < numList[minIdx])
            {
                minIdx = j;
            }
        }

        swap(numList[i], numList[minIdx]);
    }
}


////////////////////////////////////////////////////////////////////////////////
// Shell Sort
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void ShellSort(vector<T> &numList)
{
    int H = 1;

    while (H < numList.size() / 3)
    {
        H = H * 3 + 1;
    }

    while (H > 0)
    {
        for (int i = H; i < numList.size(); i++)
        {
            for (int j = i; j >= H; j -= H)
            {
                if (numList[j] < numList[j - H])
                {
                    swap(numList[j], numList[j - H]);
                }
            }
        }

        H /= 3;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Merge Sort
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void __mergeNumList(vector<T> &numList, vector<T> &tmpNumList, int leftIdx,
    int midIdx, int rightIdx)
{
    for (int i = leftIdx; i <= rightIdx; i++)
    {
        tmpNumList[i] = numList[i];
    }

    int nowLeftIdx = leftIdx, nowRightIdx = midIdx + 1;

    for (int i = leftIdx; i <= rightIdx; i++)
    {
        if (nowLeftIdx > midIdx)
        {
            numList[i] = tmpNumList[nowRightIdx++];
        }
        else if (nowRightIdx > rightIdx)
        {
            numList[i] = tmpNumList[nowLeftIdx++];
        }
        else if (tmpNumList[nowLeftIdx] < tmpNumList[nowRightIdx])
        {
            numList[i] = tmpNumList[nowLeftIdx++];
        }
        else
        {
            numList[i] = tmpNumList[nowRightIdx++];
        }
    }
}


template <typename T>
void __partialMergeSort(vector<T> &numList, vector<T> &tmpNumList,
    int leftIdx, int rightIdx)
{
    if (leftIdx >= rightIdx)
    {
        return;
    }

    int midIdx = (leftIdx + rightIdx) / 2;
    __partialMergeSort(numList, tmpNumList, leftIdx, midIdx);
    __partialMergeSort(numList, tmpNumList, midIdx + 1, rightIdx);
    __mergeNumList(numList, tmpNumList, leftIdx, midIdx, rightIdx);
}


////////////////////////////////////////////////////////////////////////////////
// Merge Sort
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void MergeSort(vector<T> &numList)
{
    vector<int> tmpNumList(numList.size());
    __partialMergeSort(numList, tmpNumList, 0, numList.size() - 1);
}


template <typename T>
void MergeSort_2(vector<T> &numList)
{
    vector<int> tmpNumList(numList.size());

    for (int sortSize = 1; sortSize < numList.size(); sortSize *= 2)
    {
        // 这里leftIdx < numList.size() - sortSize是为了防止中间索引（一倍的Size）就已经超过数组长度了
        for (int leftIdx = 0; leftIdx < numList.size() - sortSize; leftIdx += sortSize * 2)
        {
            // 这里leftIdx + sortSize - 1是因为左索引加上Size后需要-1才是正确的已经归并的片段
            // 比如1-4，Size = 2时，1 + 2 - 1 = 1~2,3~4，才是正确的
            // min函数是为了防止右边界越界
            __mergeNumList(numList, tmpNumList, leftIdx, leftIdx + sortSize - 1, min(
                leftIdx + sortSize * 2 - 1, static_cast<int>(numList.size() - 1)));
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// Double Partition
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void __doublePartition(vector<T> &numList, int leftIdx, int rightIdx)
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


////////////////////////////////////////////////////////////////////////////////
// Triple Partition
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void __triplePartition(vector<T> &numList, int leftIdx, int rightIdx)
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


////////////////////////////////////////////////////////////////////////////////
// Quick Sort
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void QuickSort(vector<T> &numList)
{
//    __doublePartition(numList, 0, numList.size() - 1);
    __triplePartition(numList, 0, numList.size() - 1);
}

////////////////////////////////////////////////////////////////////////////////
// Main Program Define
////////////////////////////////////////////////////////////////////////////////

int main()
{
    srand(time(NULL));
    vector<int> numList;

    for (int i = 0; i < 20; i++)
    {
        numList.push_back(rand() % 100);
    }

//    InsertionSort(numList);
//    SelectionSort(numList);
//    ShellSort(numList);
//    MergeSort(numList);
//    MergeSort_2(numList);
    QuickSort(numList);

    assert(is_sorted(numList.begin(), numList.end()));
}
