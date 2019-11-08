#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <cassert>
#include <algorithm>

using namespace std;

const int ALPHABET_COUNT = 256;

void LSDSort(vector<string> &strList, int charCount)
{
    array<int, ALPHABET_COUNT + 1> countArray;
    vector<string> tmpStrList(strList.size());

    for (int keyIdx = charCount - 1; keyIdx >= 0; keyIdx--)
    {
        countArray.fill(0);
        for (auto &nowStr: strList) countArray[nowStr[keyIdx] + 1]++;
        for (int i = 0; i < ALPHABET_COUNT; i++) countArray[i + 1] += countArray[i];
        for (int i = 0; i < strList.size(); i++) tmpStrList[countArray[strList[i][keyIdx]]++] = strList[i];
        strList = tmpStrList;
    }
}


int main()
{
    vector<string> strList;
    ifstream f("1.txt");
    string line;
    while (getline(f, line)) strList.push_back(line);
    f.close();

    LSDSort(strList, 8);
    assert(is_sorted(strList.begin(), strList.end()));

}