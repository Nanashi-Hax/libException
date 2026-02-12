#pragma once

#include <cstdint>

namespace Library::Debug
{
    enum class BreakpointSize : uint32_t
    {
        Invalid = 0,
        Bit8 = 1,
        Bit16 = 2,
        Bit32 = 4,
        Bit64 = 8
    };

    struct DataBreakInfo
    {
        uint32_t dar;
        uint32_t pc;
    };
}