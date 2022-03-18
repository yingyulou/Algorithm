#include <pprint/pprint>

#define __Pow(C)    (1 << (C))
#define __Mask(C)   (0xffffffff / (__Pow(__Pow(C)) + 1))
#define __Sum(N, C) ((N & __Mask(C)) + ((N >> __Pow(C)) & __Mask(C)))
#define Count(N)    (__Sum(__Sum(__Sum(__Sum(__Sum(N, 0), 1), 2), 3), 4))

int main()
{
    pprint::print(
        Count(0),
        Count(1),
        Count(2),
        Count(3),
        Count(0xffff),
        Count(0x10000),
        Count(0xffffffff)
    );
}
