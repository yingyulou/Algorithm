#include <iostream>
#include <vector>
#include <utility>

using namespace std;

template <typename T>
int partition(vector<T> &dataList, int leftIdx, int rightIdx)
{
    int nowIdx = leftIdx;

    for (int idx = leftIdx + 1; idx <= rightIdx; idx++)
    {
        if (dataList[idx] < dataList[leftIdx]) swap(dataList[idx], dataList[++nowIdx]);
    }

    swap(dataList[leftIdx], dataList[nowIdx]);

    return nowIdx;
}


template <typename T>
T nlargest(vector<T> &dataList, int n)
{
    int leftIdx = 0, rightIdx = dataList.size() - 1, partitionIdx, tarIdx = dataList.size() - n;

    while (true)
    {
        partitionIdx = partition(dataList, leftIdx, rightIdx);

        if (partitionIdx < tarIdx) leftIdx = partitionIdx + 1;
        else if (partitionIdx > tarIdx) rightIdx = partitionIdx - 1;
        else break;
    }

    return dataList[partitionIdx];
}


int main()
{
    vector<int> v {5, 4, 3, 6, 7};
    cout << nlargest(v, 3);
}