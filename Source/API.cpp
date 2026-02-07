#include "Exception/Patch.hpp"
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
}