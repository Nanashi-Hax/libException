#pragma once

#include <coreinit/kernel.h>

namespace Library::Debug::Exception
{
    void Initialize();
    void SetCallback(OSExceptionType type, OSExceptionCallbackFn function);
}