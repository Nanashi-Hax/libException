#include "Debug/Thread.hpp"

#include <coreinit/thread.h>

namespace Library::Debug
{
    OSThread* Thread::raw()
    {
        return _raw;
    }

    std::vector<Thread> Thread::all()
    {
        OSThread* osIt = OSGetCurrentThread();
        if(!osIt) return {};

        // Advance iterator to begin
        while(true)
        {
            OSThread* previous = osIt->link.prev;
            if(!previous) break;
            osIt = previous;
        }

        // Stack OSThread
        std::vector<OSThread*> osThreads;
        while(true)
        {
            OSThread* next = osIt->link.next;
            if(!next) break;
            osThreads.push_back(osIt);
            osIt = next;
        }
        
        // Stack Thread
        std::vector<Thread> threads;
        for(auto it : osThreads)
        {
            Thread t(it);
        }

        return threads;
    }

    Thread::Thread(OSThread* raw) : _raw(raw) {}
}