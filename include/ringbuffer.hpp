#pragma once

#include <condition_variable>

#include <iostream>
#include <mutex>

template<typename T>
class Ringbuffer
{
  public:

    Ringbuffer(size_t size = 10);
    ~Ringbuffer() = default;

    void Add(const T& data);
    char Get();

    void Dump() const;

    inline bool IsFull() const;
    inline bool IsEmpty() const;

    inline void Terminate();

    Ringbuffer& operator<<(T data);
    Ringbuffer& operator>>(T& data);
    Ringbuffer& operator>>(std::ostream& os);

  private:

    size_t m_Size;
    uint32_t m_End, m_Start;

    bool m_Terminating = false;

    // Shared memory
    std::unique_ptr<T[]> m_Buffer;

    // synchronization
    std::mutex m_Mtx;
    std::condition_variable m_Full;
    std::condition_variable m_Empty;
};

template<typename T>
Ringbuffer<T>::Ringbuffer(size_t size)
    : m_Size(size), m_End(0), m_Start(0),
      m_Buffer(std::unique_ptr<T[]>(new T[size]))  // Smart pointer array allocated on the heap
{}

template<typename T>
void Ringbuffer<T>::Add(const T& data)
{
    std::unique_lock lk(m_Mtx);  // Lock usable with condition variables

    // If buffer is full, thread will go into a wait state, and wait for a signal
    m_Full.wait(lk, [this] { return IsFull() == false || m_Terminating; });

    // If ringbuffer should terminate, exit fuction
    if (m_Terminating)
        return;

    // Add data to current end of ringbuffer, and increase end index
    m_Buffer[m_End++] = data;
    m_End %= m_Size;

    // Unlock the mutex, and signal a thread waiting to get a value to wake up
    lk.unlock();
    m_Empty.notify_one();
}

template<typename T>
char Ringbuffer<T>::Get()
{
    std::unique_lock lk(m_Mtx);  // Lock usable with condition variables

    // If buffer is empty, thread will go into a wait state, and wait for a signal
    m_Empty.wait(lk, [this] { return IsEmpty() == false || m_Terminating; });

    // If ringbuffer should die, exit fuction
    if (m_Terminating)
        return '\0';

    // Get data at start of ringbuffer, and increase start index
    T returnVal = m_Buffer[m_Start++];
    m_Start %= m_Size;

    // Unlock the mutex, and signal a thread waiting to add a value to wake up
    lk.unlock();
    m_Full.notify_one();

    return returnVal;
}

template<typename T>
inline bool Ringbuffer<T>::IsFull() const
{
    // Checks if increasing end index by one, taking into account the rotation, will make it so that
    // end == start
    return (m_End + 1) % m_Size == m_Start;
}

template<typename T>
inline bool Ringbuffer<T>::IsEmpty() const
{
    // If end and start are equal, the buffer is empty
    return m_End == m_Start;
}

template<typename T>
void Ringbuffer<T>::Dump() const
{
    // Debug method used to get all the contents of the buffer, regardless of start and end indices
    for (size_t i = 0; i < m_Size; ++i)
        std::cout << m_Buffer[i] << ' ';
    std::cout << std::endl;
}

template<typename T>
inline void Ringbuffer<T>::Terminate()
{
    // Ringbuffer should be destroyed, wake all threads to make them finish
    m_Terminating = true;
    m_Full.notify_all();
    m_Empty.notify_all();
}

// Operator overloads of easier use of Get() and Add()

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