#include "../include/readers.hpp"
#include "../include/ringbuffer.hpp"
#include <iostream>
#include <thread>

int main(void)
{
  Ringbuffer buffer(10);
  std::thread kb(Reader::KeyboardReader, &buffer);
  std::thread bf(Reader::BufferReader, &buffer);

  kb.join();
  bf.join();

  return 0;
}
