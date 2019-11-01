#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
    int N, i, j, ownerI, ownerJ;
    ifstream f("largeUF.txt");

    f >> N;
    vector<int> dataList(N), sizeList(N, 1);

    for (int idx = 0; idx < N; idx++)
    {
        dataList[idx] = idx;
    }

    while (f >> i >> j)
    {
        for (ownerI = i; ownerI != dataList[ownerI]; ownerI = dataList[ownerI]) {}
        for (ownerJ = j; ownerJ != dataList[ownerJ]; ownerJ = dataList[ownerJ]) {}

        if (ownerI != ownerJ)
        {
            if (sizeList[ownerI] < sizeList[ownerJ])
            {
                dataList[ownerI] = ownerJ;
                sizeList[ownerJ] += sizeList[ownerI];
            }
            else
            {
                dataList[ownerJ] = ownerI;
                sizeList[ownerI] += sizeList[ownerJ];
            }
        }
    }
}