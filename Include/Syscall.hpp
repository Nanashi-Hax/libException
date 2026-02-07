#pragma once

#include <cstdint>

extern "C" 
{
    void SC_SetIABR(uint32_t value);
    void SetIABR(uint32_t value);

    void SC_SetDABR(uint32_t value);
    void SetDABR(uint32_t value);
}