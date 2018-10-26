#ifndef __BUFFER_H
#define __BUFFER_H

#include <memory>

template <typename MemoryType>
class Buffer
{
public:

    using SizeType = uint64_t;


public:
    Buffer()
    {
        mSize = 0;
        mCapacity = 0;
    }

    Buffer(SizeType initialCapacity)
    {
        mSize = 0;
        mCapacity = 0;
    }

    ~Buffer()
    {
        /* EMPTY */
    }

    Buffer(const Buffer& other)
    {

    }

    void Swap(Buffer& other)
    {

    }

    void Fill(MemoryType content, SizeType size)
    {

    }

    void Append(MemoryType content, SizeType size)
    {

    }

    void Resize(SizeType newSize)
    {
        
    }


private:

    SizeType mSize;
    SizeType mCapacity;

    std::unique_ptr<MemoryType> mBuffer;
};

#endif