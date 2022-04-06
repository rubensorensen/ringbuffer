#include "../include/ringbuffer.hpp"

Ringbuffer::Ringbuffer(size_t size)
    : m_Size(size), m_End(0), m_Start(0),
      m_Buffer(
          std::unique_ptr<char[]>(new char[size]))  // Smart pointer array allocated on the heap
{}

void Ringbuffer::Add(const char& data)
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

char Ringbuffer::Get()
{
    std::unique_lock lk(m_Mtx);  // Lock usable with condition variables

    // If buffer is empty, thread will go into a wait state, and wait for a signal
    m_Empty.wait(lk, [this] { return IsEmpty() == false || m_Terminating; });

    // If ringbuffer should die, exit fuction
    if (m_Terminating)
        return '\0';

    // Get data at start of ringbuffer, and increase start index
    char returnVal = m_Buffer[m_Start++];
    m_Start %= m_Size;

    // Unlock the mutex, and signal a thread waiting to add a value to wake up
    lk.unlock();
    m_Full.notify_one();

    return returnVal;
}

void Ringbuffer::Dump() const
{
    // Debug method used to get all the contents of the buffer, regardless of start and end indices
    for (size_t i = 0; i < m_Size; ++i)
        std::cout << m_Buffer[i] << ' ';
    std::cout << std::endl;
}

void Ringbuffer::Terminate()
{
    // Ringbuffer should be destroyed, wake all threads to make them finish
    m_Terminating = true;
    m_Full.notify_all();
    m_Empty.notify_all();
}

// Operator overloads of easier use of Get() and Add()

Ringbuffer& Ringbuffer::operator<<(char data)
{
    Add(data);
    return *this;
}

Ringbuffer& Ringbuffer::operator>>(char& data)
{
    data = Get();
    return *this;
}

Ringbuffer& Ringbuffer::operator>>(std::ostream& os)
{
    os << Get();
    os << std::endl;
    return *this;
}