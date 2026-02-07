#pragma once

#include <vector>
#include <coreinit/thread.h>

namespace Library::Debug
{
    class Thread
    {
    public:
        OSThread* raw();

        static std::vector<Thread> all();

    private:
        Thread(OSThread* raw);

        OSThread* _raw;
    };
}