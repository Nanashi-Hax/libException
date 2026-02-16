#include <cstdint>

#include <coreinit/debug.h>
#include <coreinit/memorymap.h>
#include <vector>

#include "Breakpoint.hpp"
#include "Debug/Breakpoint.hpp"
#include "Syscall.hpp"
#include "Exception.hpp"
#include "coreinit/exception.h"

namespace Library::Debug
{
    void BreakpointManager::SetDataBreakpoint(uint32_t address, bool read, bool write, BreakpointSize size)
    {
        uint32_t mask = (1 << 0) | (1 << 1) | (1 << 2);
        address &= ~(mask);
        uint32_t enabled = true;
        uint32_t r = static_cast<uint32_t>(read);
        uint32_t w = static_cast<uint32_t>(write);
        uint32_t value = address | (enabled << 2 | w << 1 | r << 0);
        dBreakpointAddress.store(address);
        dBreakpointSize.store(static_cast<uint32_t>(size));
        dabr.store(value, std::memory_order_relaxed);
        dInfoBuffer.clear();
    }

    void BreakpointManager::UnsetDataBreakpoint()
    {
        dBreakpointAddress.store(0);
        dBreakpointSize.store(0);
        dabr.store(0, std::memory_order_relaxed);
        dInfoBuffer.clear();
    }

    void BreakpointManager::SetInstructionBreakpoint(uint32_t address)
    {
        uint32_t mask = (1 << 0) | (1 << 1);
        address &= ~(mask);
        uint32_t enabled = true;
        uint32_t value = address | (enabled << 1);
        iBreakpointAddress.store(address);
        iabr.store(value, std::memory_order_relaxed);
        iInfoBuffer.clear();
    }

    void BreakpointManager::UnsetInstructionBreakpoint()
    {
        iBreakpointAddress.store(0);
        iabr.store(0, std::memory_order_relaxed);
        iInfoBuffer.clear();
    }

    std::vector<DataBreakInfo> BreakpointManager::ConsumeDataBreakInfo()
    {
        std::vector<DataBreakInfo> vector;

        while (true)
        {
            DataBreakInfo info;
            if(!dInfoBuffer.pop(info)) break;
            vector.push_back(info);
        }

        return vector;
    }

    std::vector<InstructionBreakInfo> BreakpointManager::ConsumeInstructionBreakInfo()
    {
        std::vector<InstructionBreakInfo> vector;

        while (true)
        {
            InstructionBreakInfo info;
            if(!iInfoBuffer.pop(info)) break;
            vector.push_back(info);
        }

        return vector;
    }

    constexpr inline uint32_t OP_LWZ   = 32;
    constexpr inline uint32_t OP_LWZU  = 33;
    constexpr inline uint32_t OP_LBZ   = 34;
    constexpr inline uint32_t OP_LBZU  = 35;
    constexpr inline uint32_t OP_STW   = 36;
    constexpr inline uint32_t OP_STWU  = 37;
    constexpr inline uint32_t OP_STB   = 38;
    constexpr inline uint32_t OP_STBU  = 39;
    constexpr inline uint32_t OP_LHZ   = 40;
    constexpr inline uint32_t OP_LHZU  = 41;
    constexpr inline uint32_t OP_LHA   = 42;
    constexpr inline uint32_t OP_LHAU  = 43;
    constexpr inline uint32_t OP_STH   = 44;
    constexpr inline uint32_t OP_STHU  = 45;
    constexpr inline uint32_t OP_LMW   = 46;
    constexpr inline uint32_t OP_STMW  = 47;
    constexpr inline uint32_t OP_LFS   = 48;
    constexpr inline uint32_t OP_LFSU  = 49;
    constexpr inline uint32_t OP_LFD   = 50;
    constexpr inline uint32_t OP_LFDU  = 51;
    constexpr inline uint32_t OP_STFS  = 52;
    constexpr inline uint32_t OP_STFSU = 53;
    constexpr inline uint32_t OP_STFD  = 54;
    constexpr inline uint32_t OP_STFDU = 55;

    OSSwitchThreadCallbackFn OSSwitchThreadCallbackDefault = reinterpret_cast<OSSwitchThreadCallbackFn>(0x0103C4B4);

    void BreakpointManager::Initialize()
    {
        Exception::SetCallback(OS_EXCEPTION_TYPE_DSI, DSIHandler);
        Exception::SetCallback(OS_EXCEPTION_TYPE_BREAKPOINT, BreakpointHandler);
        Exception::SetCallback(OS_EXCEPTION_TYPE_TRACE, TraceHandler);

        SetSwitchThreadCallback(SwitchThreadHandler);
        isInitialized = true;
    }

    void BreakpointManager::Shutdown()
    {
        isInitialized = false;
        SetSwitchThreadCallback(OSSwitchThreadCallbackDefault);
    }

    bool BreakpointManager::IsInitialized()
    {
        return isInitialized;
    }

    void BreakpointManager::SetDABR(uint32_t value)
    {
        ::SetDABR(value);
    }

    void BreakpointManager::SetIABR(uint32_t value)
    {
        ::SetIABR(value);
    }

    void BreakpointManager::SetSwitchThreadCallback(OSSwitchThreadCallbackFn function)
    {
        OSSetSwitchThreadCallback(function);
    }

    BOOL BreakpointManager::DSIHandler(OSContext* context)
    {
        if ((context->dsisr & (MATCH_DABR_BIT)) == 0)
        {
            OSFatal("DSI");
            return FALSE;
        }

        if (!context) return FALSE;
        
        uint32_t dar = context->dar;

        uint32_t size = dBreakpointSize.load();
        uint32_t begin = dBreakpointAddress.load();
        uint32_t end = begin + size;
        
        if((begin <= dar || dar < end))
        {
            DataBreakInfo info =
            {
                context->dar,
                context->srr0
            };
            dInfoBuffer.push(info);
        }
    
        SetDABR(0);
        context->srr1 |= SINGLE_STEP_BIT;
        return TRUE;
    }

    BOOL BreakpointManager::BreakpointHandler(OSContext* context)
    {
        if (!context) return FALSE;
        
        uint32_t pc = context->srr0;

        uint32_t address = iBreakpointAddress.load();
        
        if(address == pc)
        {
            InstructionBreakInfo info =
            {
                context->srr0
            };
            iInfoBuffer.push(info);
        }
    
        SetIABR(0);
        context->srr1 |= SINGLE_STEP_BIT;
        return TRUE;
    }

    BOOL BreakpointManager::TraceHandler(OSContext* context)
    {
        if(context->srr1 & SINGLE_STEP_BIT)
        {
            SetDABR(dabr.load());
            SetIABR(iabr.load());
            context->srr1 &= ~SINGLE_STEP_BIT;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    void BreakpointManager::SwitchThreadHandler(OSThread* thread, OSThreadQueue*)
    {
        if (!thread) return;

        uint32_t d = dabr.load();
        uint32_t i = iabr.load();
        uint32_t addr = reinterpret_cast<uint32_t>(thread);

        uint32_t* dPrev = dMap.find(addr);
        if(!dPrev)
        {
            SetDABR(d);
            dMap.insert(addr, d);
        }
        else if(*dPrev != d)
        {
            SetDABR(d);
            *dPrev = d;
        }

        uint32_t* iPrev = iMap.find(addr);
        if(!iPrev)
        {
            SetIABR(i);
            iMap.insert(addr, i);
        }
        else if(*iPrev != i)
        {
            SetIABR(i);
            *iPrev = i;
        }
    }
}