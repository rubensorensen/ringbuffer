#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <stddef.h>
#include <stdint.h>

class Ringbuffer {
  public:
  Ringbuffer(size_t size);
  ~Ringbuffer();

  void Add(const char& data);
  char Get();

  private:
  char* m_Buffer;
  uint32_t m_Size;
  uint32_t m_In, m_Out;
};

#endif
