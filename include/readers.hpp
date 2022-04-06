#pragma once

#include "ringbuffer.hpp"

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

namespace Reader
{
    void BufferReader(Ringbuffer& buffer, bool& shouldExit)
    {
        while (!shouldExit)
            buffer >> std::cout;  // Overloaded operator: Calls buffet.Get() and sends output to
                                  // standard output
    }

    void KeyboardReader(Ringbuffer& buffer, bool& shouldExit)
    {
        std::string input;
        // Read input from cin to input, exit when input is 0
        while (std::getline(std::cin, input), input != "0")
            for (char c : input)
                buffer << c;  // Overloaded operator: Calls buffet.Add(c)
        shouldExit = true;
        buffer.Terminate();  // Buffer should terminate
    }

    void StringReader(Ringbuffer& buffer, bool& shouldExit)
    {
        const char* str =
            "According to all known laws of aviation, there is no way a bee should be able to fly";
        size_t index = 0;

        // Go thorugh str and add all characters to buffer with an interval of one second
        while (!shouldExit && index < strlen(str))
        {
            buffer << str[index++];
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}  // namespace Reader
