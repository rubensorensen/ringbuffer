#pragma once

#include <condition_variable>

#include <iostream>
#include <memory>
#include <mutex>
#include <stddef.h>
#include <stdint.h>

template<typename T>
class Ringbuffer
{
  public:

    Ringbuffer(size_t size = 10);
    ~Ringbuffer();

    void Add(const T& data);
    char Get();

    void Dump() const;

    inline bool IsFull() const;
    inline bool IsEmpty() const;

    inline void Die();

    Ringbuffer& operator<<(T data);
    Ringbuffer& operator>>(T& data);
    Ringbuffer& operator>>(std::ostream& os);

  private:

    std::unique_ptr<T[]> m_Buffer;
    size_t m_Size;
    uint32_t m_In, m_Out;

    bool m_Dying = false;

    // synchronization
    std::mutex m_Mtx;
    std::condition_variable m_Full;
    std::condition_variable m_Empty;
};

template<typename T>
Ringbuffer<T>::Ringbuffer(size_t size)
    : m_Buffer(std::unique_ptr<T[]>(new T[size])), m_Size(size), m_In(0), m_Out(0)
{}

template<typename T>
Ringbuffer<T>::~Ringbuffer()
{}

template<typename T>
void Ringbuffer<T>::Add(const T& data)
{
    std::unique_lock lk(m_Mtx);
    m_Full.wait(lk, [this] { return IsFull() == false || m_Dying; });

    // If ringbuffer should die, exit fuction
    if (m_Dying)
        return;

    m_Buffer[m_In++] = data;
    m_In %= m_Size;

    lk.unlock();
    m_Empty.notify_one();
}

template<typename T>
char Ringbuffer<T>::Get()
{
    std::unique_lock lk(m_Mtx);
    m_Empty.wait(lk, [this] { return IsEmpty() == false || m_Dying; });

    // If ringbuffer should die, exit fuction
    if (m_Dying)
        return '\0';

    T returnVal = m_Buffer[m_Out++];
    m_Out %= m_Size;

    lk.unlock();
    m_Full.notify_one();

    return returnVal;
}

template<typename T>
inline bool Ringbuffer<T>::IsFull() const
{
    return (m_In + 1) % m_Size == m_Out;
}

template<typename T>
inline bool Ringbuffer<T>::IsEmpty() const
{
    return m_In == m_Out;
}

template<typename T>
void Ringbuffer<T>::Dump() const
{
    for (size_t i = 0; i < m_Size; ++i)
        std::cout << m_Buffer[i] << ' ';
    std::cout << std::endl;
}

template<typename T>
inline void Ringbuffer<T>::Die()
{
    m_Dying = true;
    m_Full.notify_all();
    m_Empty.notify_all();
}

template<typename T>
Ringbuffer<T>& Ringbuffer<T>::operator<<(T data)
{
    Add(data);
    return *this;
}

template<typename T>
Ringbuffer<T>& Ringbuffer<T>::operator>>(T& data)
{
    data = Get();
    return *this;
}

template<typename T>
Ringbuffer<T>& Ringbuffer<T>::operator>>(std::ostream& os)
{
    os << Get();
    os << std::endl;
    return *this;
}