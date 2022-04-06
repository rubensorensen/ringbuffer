#include "../include/readers.hpp"

int main(void)
{
    Ringbuffer buffer(10);

    // Program runs while shouldExit is false
    bool shouldExit = false;

    // J-threads, introduced in C++ 20, automatically joins on destruction
    std::jthread kb(Reader::KeyboardReader, std::ref(buffer), std::ref(shouldExit));
    std::jthread bf(Reader::BufferReader, std::ref(buffer), std::ref(shouldExit));
    std::jthread cw(Reader::CharWriter, std::ref(buffer), std::ref(shouldExit));

    return EXIT_SUCCESS;
}
