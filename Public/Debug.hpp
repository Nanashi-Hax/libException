#pragma once

#include <cstdint>
#include <coreinit/thread.h>

#include "Debug/Thread.hpp"

namespace Library::Debug
{
    void Initialize();
    
    void SetIABR(uint32_t value);
    void SetDABR(uint32_t value);
}