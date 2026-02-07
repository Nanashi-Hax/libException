#pragma once

#include <cstdint>
#include <vector>
#include <coreinit/thread.h>

namespace Library::Debug
{
    void InitializeException();
    
    void SetIABR(uint32_t value);
    void SetDABR(uint32_t value);

    std::vector<OSThread*> GetAllThread();
}