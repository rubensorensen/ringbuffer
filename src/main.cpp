#include "../include/readers.hpp"
#include "../include/ringbuffer.hpp"

int main(void)
{
    Ringbuffer<char> buffer(10);
    bool shouldExit = false;

    // J-threads, introduced in C++ 20, automatically joins on destruction
    std::jthread kb(Reader::KeyboardReader, std::ref(buffer), std::ref(shouldExit));
    std::jthread bf(Reader::BufferReader, std::ref(buffer), std::ref(shouldExit));
    std::jthread cw(Reader::CharWriter, std::ref(buffer), std::ref(shouldExit));

    return EXIT_SUCCESS;
}
