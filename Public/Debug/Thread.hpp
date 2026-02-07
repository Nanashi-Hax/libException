#pragma once

#include <vector>
#include <coreinit/thread.h>

namespace Library::Debug
{
    class Thread
    {
    public:
        Thread(OSThread* os);

        static std::vector<Thread> all();

    private:
        OSThread* os;
    };
}