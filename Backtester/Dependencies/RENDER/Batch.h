#pragma once

#include <vector>

template<typename T>
class Batch{
private:
    T* m_pBatch;
    unsigned int m_LastCapacity;
    unsigned int m_InternalCounter;

    void ResizeBatch(int newCapacity);
public:
    Batch();
    ~Batch();

    void Reset()
    {
        m_InternalCounter = 0;
    }

    void doHandleOverflow()
    {
        unsigned int lNewCapacity = m_LastCapacity + 20;

        ResizeBatch(lNewCapacity);

        m_LastCapacity = lNewCapacity;
    }

    void HandleOverflow()
    {
        if(m_InternalCounter + 1 >= m_LastCapacity)
            doHandleOverflow();
    }

    void Push(const T& obj)
    {
//        LOGI("Pushing new Internal %d", m_InternalCounter);
        HandleOverflow();

        m_pBatch[m_InternalCounter++] = obj;
    }

    bool Empty()
    {
        return m_InternalCounter == 0;
    }

    unsigned int getSizeInBytes()
    {
        return getSize() * sizeof(T);
    }

    const T* getData()
    {
        return m_pBatch;
    }

    unsigned int getSize();

    unsigned int getCount()
    {
        return getSize();
    }
};

template<typename T>
void Batch<T>::ResizeBatch(int newCapacity)
{
//    LOGI("Resizeing From %d to %d capacity", m_LastCapacity, newCapacity);
    //m_Batch.reserve(newCapacity);
    if (m_pBatch)
        m_pBatch = (T*)realloc(m_pBatch, newCapacity * sizeof(T));
    else
        m_pBatch = (T*)malloc(newCapacity * sizeof(T));
}

template<typename T>
unsigned int Batch<T>::getSize()
{
    return (m_InternalCounter + 1);
}

template<typename T>
Batch<T>::Batch()
        : m_LastCapacity(50)
        , m_InternalCounter(0)
    , m_pBatch(nullptr)
{
    ResizeBatch(m_LastCapacity);
}

template<typename T>
inline Batch<T>::~Batch()
{
    if (m_pBatch)
        free((void*)m_pBatch);
}
