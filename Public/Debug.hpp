#pragma once

#include <cstdint>

#include <coreinit/exception.h>
#include <coreinit/thread.h>

namespace Library::Debug
{
    extern "C"
    {
        using OSSwitchThreadCallbackFn = void (*)(OSThread* thread, OSThreadQueue * queue);
        OSSwitchThreadCallbackFn OSSetSwitchThreadCallback(OSSwitchThreadCallbackFn callback);
        extern OSSwitchThreadCallbackFn OSSwitchThreadCallbackDefault;
    }

    void Initialize();
    
    void SetIABR(uint32_t value);
    void SetDABR(uint32_t value);

    void SetDSICallback(OSExceptionCallbackFn callback);
    void SetSwitchThreadCallback(OSSwitchThreadCallbackFn function);

    void ExcecuteInstruction(OSContext* context, uint32_t instruction);
}