#include "../include/ringbuffer.hpp"

Ringbuffer::Ringbuffer(size_t size)
    : m_Buffer(new char[size]), m_Size(size), m_In(0), m_Out(0)
{
}

Ringbuffer::~Ringbuffer() { delete[] m_Buffer; }

void Ringbuffer::Add(const char& data)
{
  while ((m_In + 1) % m_Size == m_Out)
    ;
  m_Buffer[m_In] = {data};
  m_In = {(m_In + 1) % m_Size};
}

char Ringbuffer::Get()
{
  while (m_In == m_Out)
    ;
  char n{m_Buffer[m_Out]};
  m_Out = {(m_Out + 1) % m_Size};
  return n;
}
