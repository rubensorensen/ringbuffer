#include "../include/readers.hpp"
#include <iostream>

namespace Reader {
  void BufferReader(Ringbuffer* buffer)
  {
    while (true)
      std::cout << buffer->Get();
  }

  void KeyboardReader(Ringbuffer* buffer)
  {
    while (true) {
      std::string input;
      std::getline(std::cin, input);

      for (uint32_t i{0}; i < input.size(); ++i)
        buffer->Add(input[i]);
      buffer->Add('\n');
    }
  }
} // namespace Reader
