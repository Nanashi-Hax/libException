#pragma once

#include <cstdint>
#include <coreinit/context.h>

constexpr inline uint32_t OP_LWZ  = 32;
constexpr inline uint32_t OP_LWZU = 33;
constexpr inline uint32_t OP_LBZ  = 34;
constexpr inline uint32_t OP_LBZU = 35;
constexpr inline uint32_t OP_STW  = 36;
constexpr inline uint32_t OP_STWU = 37;
constexpr inline uint32_t OP_STB  = 38;
constexpr inline uint32_t OP_STBU = 39;
constexpr inline uint32_t OP_LHZ  = 40;
constexpr inline uint32_t OP_LHZU = 41;
constexpr inline uint32_t OP_LHA  = 42;
constexpr inline uint32_t OP_LHAU = 43;
constexpr inline uint32_t OP_STH  = 44;
constexpr inline uint32_t OP_STHU = 45;
constexpr inline uint32_t OP_LMW  = 46;
constexpr inline uint32_t OP_STMW = 47;

namespace Library::Debug
{
    void ExcecuteInstructionImpl(OSContext* context, uint32_t instruction);
}