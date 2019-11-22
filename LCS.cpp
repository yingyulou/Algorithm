#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int LCS(const string &strA, const string &strB)
{
    vector<vector<int>> lcsList(strA.size() + 1, vector<int>(strB.size() + 1));

    for (int strLenA = 1; strLenA <= strA.size(); strLenA++)
    {
        for (int strLenB = 1; strLenB <= strB.size(); strLenB++)
        {
            if (strA[strLenA - 1] == strB[strLenB - 1])
            {
                lcsList[strLenA][strLenB] = lcsList[strLenA - 1][strLenB - 1] + 1;
            }
            else
            {
                lcsList[strLenA][strLenB] = max(lcsList[strLenA - 1][strLenB],
                    lcsList[strLenA][strLenB - 1]);
            }
        }
    }

    return lcsList.back().back();
}


int main()
{
    cout << LCS("ABCBDAB", "BDCABA") << endl;
}