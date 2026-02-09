#include "Debug/Thread.hpp"

#include <coreinit/thread.h>

namespace Library::Debug
{
    std::string Thread::name()
    {
        return _name;
    }

    uint16_t Thread::id()
    {
        return _id;
    }

    std::vector<Thread> Thread::all()
    {
        OSThread* it = OSGetCurrentThread();
        if (!it) return {};

        while (it->link.prev)
        {
            it = it->link.prev;
        }

        std::vector<Thread> threads;

        for (OSThread* cur = it; cur; cur = cur->link.next)
        {
            Thread t(cur->name, cur->id);
            threads.emplace_back(t);
        }

        return threads;
    }

    Thread::Thread(std::string name, uint16_t id) : _name(name), _id(id) {}
}