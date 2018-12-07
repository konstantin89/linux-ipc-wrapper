#ifndef __BUFFER_H
#define __BUFFER_H

#include <iostream>
#include <memory>
#include <vector>
#include <type_traits>

template <typename MemoryPointerType>
class Buffer
{
public:

    using MemoryType = typename std::remove_pointer<MemoryPointerType>::type;

    using SizeType = uint64_t;
    using iterator = typename std::vector<MemoryType>::iterator;
    using const_iterator = typename std::vector<MemoryType>::const_iterator;


public:
    Buffer()
    {

    }

    Buffer(SizeType initialCapacity)
    {
        mBuffer.reserve(initialCapacity);
    }

    ~Buffer()
    {
        /* EMPTY */
    }

    Buffer(const Buffer& other)
    {
        mBuffer = other.mBuffer;
    }

    void Swap(Buffer& other)
    {
        mBuffer.swap(other.mBuffer);
    }

    void Insert(MemoryPointerType content, SizeType size)
    {
        mBuffer.clear();
        mBuffer.insert(mBuffer.begin(), content, content + size);
    }

    void Append(MemoryPointerType content, SizeType size)
    {
        mBuffer.insert(mBuffer.end(), content, content + size);
    }

    void Clear()
    {
        mBuffer.clear();
    }

    void Reserve(SizeType aSize)
    {
        mBuffer.reserve(aSize);
    }

    iterator Begin()
    {
        return mBuffer.begin();
    }

    iterator End()
    {
        return mBuffer.end();
    }

    SizeType Size()
    {
        return mBuffer.size();
    }

    SizeType Capacity()
    {
        return mBuffer.capacity();
    }

    MemoryPointerType Front()
    {
        return &mBuffer[0];
    }

    void PrintAsUnicode()
    {
        for(auto elem : mBuffer)
        {
            std::cout << elem;
        }
        std::cout << std::endl;
    }

    std::string GetBufferAsString()
    {
        return std::string(Front(), Size());
    }

private:

    std::vector<MemoryType> mBuffer;
};

#endif