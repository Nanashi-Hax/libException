#include "Debug/Assembly.hpp"
#include <coreinit/debug.h>
#include <coreinit/memorymap.h>
#include <coreinit/context.h>
#include <string>
#include <format>

namespace Library::Debug
{
    void ExcecuteInstructionImpl(OSContext* context, uint32_t instruction)
    {
        if (!context) return;

        uint32_t op   = (instruction >> 26) & 0x3F;
        uint32_t rD   = (instruction >> 21) & 0x1F;
        uint32_t rA   = (instruction >> 16) & 0x1F;
        int16_t simm  = static_cast<int16_t>(instruction & 0xFFFF);

        uint32_t ea;
        if (rA == 0) ea = (int32_t)simm;
        else ea = context->gpr[rA] + (int32_t)simm;

        if (!OSIsAddressValid(ea)) return;

        switch (op)
        {
            case OP_LWZ:
            case OP_LWZU:
            {
                uint32_t value = *(uint32_t*)ea;
                context->gpr[rD] = value;

                if (op == OP_LWZU) context->gpr[rA] = ea;

                return;
            }

            case OP_LBZ:
            case OP_LBZU:
            {
                uint32_t value = *(uint8_t*)ea;
                context->gpr[rD] = value;

                if (op == OP_LBZU) context->gpr[rA] = ea;

                return;
            }

            case OP_LHZ:
            case OP_LHZU:
            {
                uint32_t value = *(uint16_t*)ea;
                context->gpr[rD] = value;

                if (op == OP_LHZU) context->gpr[rA] = ea;

                return;
            }

            case OP_STW:
            case OP_STWU:
            {
                uint32_t value = context->gpr[rD];
                *(uint32_t*)ea = value;

                if (op == OP_STWU) context->gpr[rA] = ea;

                return;
            }

            case OP_STB:
            case OP_STBU:
            {
                uint32_t value = context->gpr[rD];
                *(uint8_t*)ea = value;

                if (op == OP_STBU) context->gpr[rA] = ea;

                return;
            }

            case OP_STH:
            case OP_STHU:
            {
                uint32_t value = context->gpr[rD];
                *(uint16_t*)ea = value;

                if (op == OP_STHU) context->gpr[rA] = ea;

                return;
            }

            default:
            {
                std::string msg = std::format("Instruction: {:08X}", instruction);
                OSFatal(msg.c_str());
                return;
            }
        }
    }
}