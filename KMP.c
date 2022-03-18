#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// Get Backward List
////////////////////////////////////////////////////////////////////////////////

size_t *__getBackwardList(const char *patternStr)
{
    size_t *backwardList = (size_t *)calloc(strlen(patternStr), sizeof(size_t));

    for (size_t patternIdx = 1, backwardIdx = 0; patternStr[patternIdx]; patternIdx++)
    {
        for (; backwardIdx && patternStr[patternIdx] != patternStr[backwardIdx];
            backwardIdx = backwardList[backwardIdx]);

        backwardList[patternIdx] = backwardIdx +=
            patternStr[patternIdx] == patternStr[backwardIdx];
    }

    return backwardList;
}


////////////////////////////////////////////////////////////////////////////////
// KMP
////////////////////////////////////////////////////////////////////////////////

void KMP(const char *searchStr, const char *patternStr)
{
    size_t *backwardList = __getBackwardList(patternStr);

    for (size_t searchIdx = 0, patternIdx = 0; searchIdx < strlen(searchStr); searchIdx++)
    {
        for (; patternIdx && searchStr[searchIdx] != patternStr[patternIdx];
            patternIdx = backwardList[patternIdx]);

        if (searchStr[searchIdx] == patternStr[patternIdx])
        {
            if (patternIdx + 1 == strlen(patternStr))
            {
                printf("%d\n", searchIdx - patternIdx);

                patternIdx = backwardList[patternIdx];
            }
            else
            {
                patternIdx++;
            }
        }
    }

    free(backwardList);
}


////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

int main()
{
    KMP("abababab", "abab");
}
