#include <iostream>
#include <vector>
#include <array>

using namespace std;

int cutRod(const vector<int> &priceList, int rodLen)
{
    vector<int> maxPriceList(rodLen + 1);
    int nowPrice, maxPrice;

    for (int nowRodLen = 1; nowRodLen <= rodLen; nowRodLen++)
    {
        maxPrice = -1;

        for (int cutLen = 1; cutLen <= nowRodLen; cutLen++)
        {
            nowPrice = priceList[cutLen] + maxPriceList[nowRodLen - cutLen];
            if (nowPrice > maxPrice) maxPrice = nowPrice;
        }

        maxPriceList[nowRodLen] = maxPrice;
    }

    return maxPriceList[rodLen];
}


int main()
{
    vector<int> priceList {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    for (int rodLen = 1; rodLen <= 10; rodLen++) cout << cutRod(priceList, rodLen) << endl;
}