#pragma once

#include "ringbuffer.hpp"

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

namespace Reader
{
    void BufferReader(Ringbuffer<char>& buffer, bool& shouldExit)
    {
        while (!shouldExit)
            buffer >> std::cout;
    }

    void KeyboardReader(Ringbuffer<char>& buffer, bool& shouldExit)
    {
        std::string input;
        // Read input from cin to input, exit when input is 0
        while (std::getline(std::cin, input), input != "0")
            for (char c : input)
                buffer << c;
        shouldExit = true;
        buffer.Die();
    }

    void CharWriter(Ringbuffer<char>& buffer, bool& shouldExit)
    {
        const char* str =
            "According to all known laws of aviation, there is no way a bee should be able to fly";
        size_t index = 0;
        while (!shouldExit && index < strlen(str))
        {
            buffer << str[index++];
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}  // namespace Reader
