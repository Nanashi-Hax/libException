#include "Exception/Patch.hpp"
#include "Debug.hpp"
#include "Debug/Assembly.hpp"
#include "Syscall.hpp"

#include <cstdint>
#include <kernel/kernel.h>

namespace Library::Debug
{
    void Initialize()
    {
        Patch::apply();

        //KernelPatchSyscall(0x6D, reinterpret_cast<uint32_t>(&SC_SetIABR));
        KernelPatchSyscall(0x6D, reinterpret_cast<uint32_t>(&SC_SetDABR));
    }

    void SetIABR(uint32_t value)
    {
        ::SetIABR(value);
    }

    void SetDABR(uint32_t value)
    {
        ::SetDABR(value);
    }

    void SetSwitchThreadCallback(OSSwitchThreadCallbackFn function)
    {
        OSSetSwitchThreadCallback(function);
    }

    void SetDSICallback(OSExceptionCallbackFn callback)
    {
        OSSetExceptionCallbackEx(OSExceptionMode::OS_EXCEPTION_MODE_THREAD_ALL_CORES, OSExceptionType::OS_EXCEPTION_TYPE_DSI, callback);
    }

    OSSwitchThreadCallbackFn OSSwitchThreadCallbackDefault = reinterpret_cast<OSSwitchThreadCallbackFn>(0x0103C4B4);

    void ExcecuteInstruction(OSContext* context, uint32_t instruction)
    {
        ExcecuteInstructionImpl(context, instruction);
    }
}