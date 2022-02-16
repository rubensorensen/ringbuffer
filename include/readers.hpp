#ifndef READERS_HPP
#define READERS_HPP

#include "ringbuffer.hpp"

namespace Reader {
  void BufferReader(Ringbuffer* buffer);
  void KeyboardReader(Ringbuffer* buffer);
} // namespace Reader

#endif
