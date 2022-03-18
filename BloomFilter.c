#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
// Struct BloomFilter
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    unsigned char *__bitmap;
    size_t         __size;
    int            __salt1;
    int            __salt2;
    int            __salt3;
    int            __salt4;
    int            __salt5;
    int            __salt6;
} BloomFilter;


////////////////////////////////////////////////////////////////////////////////
// BloomFilter Constructor
////////////////////////////////////////////////////////////////////////////////

void bloomFilterConstructor(BloomFilter *this, size_t size)
{
    srand(time(0));

    this->__bitmap = (unsigned char *)calloc(size, sizeof(unsigned char));
    this->__size   = size;
    this->__salt1  = rand();
    this->__salt2  = rand();
    this->__salt3  = rand();
    this->__salt4  = rand();
    this->__salt5  = rand();
    this->__salt6  = rand();
}


////////////////////////////////////////////////////////////////////////////////
// Bitmap Get
////////////////////////////////////////////////////////////////////////////////

_Bool __bitmapGet(char *bitmapPtr, size_t idx)
{
    return bitmapPtr[idx / 8] & (1 << idx % 8);
}


////////////////////////////////////////////////////////////////////////////////
// Bitmap Set
////////////////////////////////////////////////////////////////////////////////

void __bitmapSet(char *bitmapPtr, size_t idx)
{
    bitmapPtr[idx / 8] |= (1 << idx % 8);
}


////////////////////////////////////////////////////////////////////////////////
// Int Hash Function With Salt
////////////////////////////////////////////////////////////////////////////////

size_t __intHashFunctionWithSalt(int val, int salt)
{
    return val ^ salt;
}


////////////////////////////////////////////////////////////////////////////////
// BloomFilter Find
////////////////////////////////////////////////////////////////////////////////

_Bool bloomFilterFind(BloomFilter *this, int val)
{
    return
        __bitmapGet(this->__bitmap, __intHashFunctionWithSalt(val, this->__salt1) % (this->__size << 3)) &&
        __bitmapGet(this->__bitmap, __intHashFunctionWithSalt(val, this->__salt2) % (this->__size << 3)) &&
        __bitmapGet(this->__bitmap, __intHashFunctionWithSalt(val, this->__salt3) % (this->__size << 3)) &&
        __bitmapGet(this->__bitmap, __intHashFunctionWithSalt(val, this->__salt4) % (this->__size << 3)) &&
        __bitmapGet(this->__bitmap, __intHashFunctionWithSalt(val, this->__salt5) % (this->__size << 3)) &&
        __bitmapGet(this->__bitmap, __intHashFunctionWithSalt(val, this->__salt6) % (this->__size << 3));
}


////////////////////////////////////////////////////////////////////////////////
// BloomFilter Add
////////////////////////////////////////////////////////////////////////////////

void bloomFilterAdd(BloomFilter *this, int val)
{
    __bitmapSet(this->__bitmap, __intHashFunctionWithSalt(val, this->__salt1) % (this->__size << 3));
    __bitmapSet(this->__bitmap, __intHashFunctionWithSalt(val, this->__salt2) % (this->__size << 3));
    __bitmapSet(this->__bitmap, __intHashFunctionWithSalt(val, this->__salt3) % (this->__size << 3));
    __bitmapSet(this->__bitmap, __intHashFunctionWithSalt(val, this->__salt4) % (this->__size << 3));
    __bitmapSet(this->__bitmap, __intHashFunctionWithSalt(val, this->__salt5) % (this->__size << 3));
    __bitmapSet(this->__bitmap, __intHashFunctionWithSalt(val, this->__salt6) % (this->__size << 3));
}


////////////////////////////////////////////////////////////////////////////////
// BloomFilter Destructor
////////////////////////////////////////////////////////////////////////////////

void bloomFilterDestructor(BloomFilter *this)
{
    free(this->__bitmap);
}


////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

int main()
{
    BloomFilter bloomFilterObj;

    bloomFilterConstructor(&bloomFilterObj, 0x1000);

    for (int i = 0; i < 10; i++)
    {
        bloomFilterAdd(&bloomFilterObj, i);
    }

    for (int i = 0; i < 20; i++)
    {
        printf("%d\n", bloomFilterFind(&bloomFilterObj, i));
    }

    bloomFilterDestructor(&bloomFilterObj);
}
