#pragma once

#include <condition_variable>

#include <iostream>
#include <mutex>

class Ringbuffer
{
  public:

    Ringbuffer(size_t size = 10);
    ~Ringbuffer() = default;

    void Add(const char& data);
    char Get();

    void Dump() const;

    inline bool IsFull() const { return (m_End + 1) % m_Size == m_Start; }
    inline bool IsEmpty() const { return m_End == m_Start; }
    void Terminate();

    Ringbuffer& operator<<(char data);
    Ringbuffer& operator>>(char& data);
    Ringbuffer& operator>>(std::ostream& os);

  private:

    size_t m_Size;
    uint32_t m_End, m_Start;

    bool m_Terminating = false;

    // Shared memory
    std::unique_ptr<char[]> m_Buffer;

    // synchronization
    std::mutex m_Mtx;
    std::condition_variable m_Full;
    std::condition_variable m_Empty;
};