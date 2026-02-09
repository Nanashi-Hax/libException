#pragma once

#include <vector>
#include <string>
#include <coreinit/thread.h>

namespace Library::Debug
{
    class Thread
    {
    public:
        static std::vector<Thread> all();

        std::string name();
        uint16_t id();

    private:
        Thread(std::string name, uint16_t id);

        std::string _name;
        uint16_t _id;
    };
}