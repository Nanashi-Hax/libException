#include "coreinit/exception.h"
#include <string>
#include <coreinit/kernel.h>
#include <coreinit/debug.h>

namespace Library::Debug::Exception
{
    static uint8_t sStack[0x1000];
    OSContext sContext;

    namespace Callback
    {
        OSExceptionCallbackFn systemReset;
        OSExceptionCallbackFn machineCheck;
        OSExceptionCallbackFn dsi;
        OSExceptionCallbackFn isi;
        OSExceptionCallbackFn externalInterrupt;
        OSExceptionCallbackFn alignment;
        OSExceptionCallbackFn program;
        OSExceptionCallbackFn floatingPoint;
        OSExceptionCallbackFn decrementer;
        OSExceptionCallbackFn systemCall;
        OSExceptionCallbackFn trace;
        OSExceptionCallbackFn performanceMonitor;
        OSExceptionCallbackFn breakpoint;
        OSExceptionCallbackFn systemInterrupt;
        OSExceptionCallbackFn ici;
    }

    namespace ChainInfo
    {
        OSExceptionChainInfo systemReset;
        OSExceptionChainInfo machineCheck;
        OSExceptionChainInfo dsi;
        OSExceptionChainInfo isi;
        OSExceptionChainInfo externalInterrupt;
        OSExceptionChainInfo alignment;
        OSExceptionChainInfo program;
        OSExceptionChainInfo floatingPoint;
        OSExceptionChainInfo decrementer;
        OSExceptionChainInfo systemCall;
        OSExceptionChainInfo trace;
        OSExceptionChainInfo performanceMonitor;
        OSExceptionChainInfo breakpoint;
        OSExceptionChainInfo systemInterrupt;
        OSExceptionChainInfo ici;
    }

    OSExceptionCallbackFn GetCallback(OSExceptionType type)
    {
        switch (type)
        {
            case OS_EXCEPTION_TYPE_SYSTEM_RESET: return Callback::systemReset;
            case OS_EXCEPTION_TYPE_MACHINE_CHECK: return Callback::machineCheck;
            case OS_EXCEPTION_TYPE_DSI: return Callback::dsi;
            case OS_EXCEPTION_TYPE_ISI: return Callback::isi;
            case OS_EXCEPTION_TYPE_EXTERNAL_INTERRUPT: return Callback::externalInterrupt;
            case OS_EXCEPTION_TYPE_ALIGNMENT: return Callback::alignment;
            case OS_EXCEPTION_TYPE_PROGRAM: return Callback::program;
            case OS_EXCEPTION_TYPE_FLOATING_POINT: return Callback::floatingPoint;
            case OS_EXCEPTION_TYPE_DECREMENTER: return Callback::decrementer;
            case OS_EXCEPTION_TYPE_SYSTEM_CALL: return Callback::systemCall;
            case OS_EXCEPTION_TYPE_TRACE: return Callback::trace;
            case OS_EXCEPTION_TYPE_PERFORMANCE_MONITOR: return Callback::performanceMonitor;
            case OS_EXCEPTION_TYPE_BREAKPOINT: return Callback::breakpoint;
            case OS_EXCEPTION_TYPE_SYSTEM_INTERRUPT: return Callback::systemInterrupt;
            case OS_EXCEPTION_TYPE_ICI: return Callback::ici;
            default: return nullptr;
        }
    }

    char const * GetString(OSExceptionType type)
    {
        switch (type)
        {
            case OS_EXCEPTION_TYPE_SYSTEM_RESET: return "SystemReset";
            case OS_EXCEPTION_TYPE_MACHINE_CHECK: return "MachineCheck";
            case OS_EXCEPTION_TYPE_DSI: return "DSI";
            case OS_EXCEPTION_TYPE_ISI: return "ISI";
            case OS_EXCEPTION_TYPE_EXTERNAL_INTERRUPT: return "ExternalInterrupt";
            case OS_EXCEPTION_TYPE_ALIGNMENT: return "Alignment";
            case OS_EXCEPTION_TYPE_PROGRAM: return "Program";
            case OS_EXCEPTION_TYPE_FLOATING_POINT: return "FloatingPoint";
            case OS_EXCEPTION_TYPE_DECREMENTER: return "Decrementer";
            case OS_EXCEPTION_TYPE_SYSTEM_CALL: return "SystemCall";
            case OS_EXCEPTION_TYPE_TRACE: return "Trace";
            case OS_EXCEPTION_TYPE_PERFORMANCE_MONITOR: return "PerformanceMonitor";
            case OS_EXCEPTION_TYPE_BREAKPOINT: return "Breakpoint";
            case OS_EXCEPTION_TYPE_SYSTEM_INTERRUPT: return "SystemInterrupt";
            case OS_EXCEPTION_TYPE_ICI: return "ICI";
            default: return "Unknown";
        }
    }

    OSExceptionChainInfo * GetChainInfo(OSExceptionType type)
    {
        switch (type)
        {
            case OS_EXCEPTION_TYPE_SYSTEM_RESET: return &ChainInfo::systemReset;
            case OS_EXCEPTION_TYPE_MACHINE_CHECK: return &ChainInfo::machineCheck;
            case OS_EXCEPTION_TYPE_DSI: return &ChainInfo::dsi;
            case OS_EXCEPTION_TYPE_ISI: return &ChainInfo::isi;
            case OS_EXCEPTION_TYPE_EXTERNAL_INTERRUPT: return &ChainInfo::externalInterrupt;
            case OS_EXCEPTION_TYPE_ALIGNMENT: return &ChainInfo::alignment;
            case OS_EXCEPTION_TYPE_PROGRAM: return &ChainInfo::program;
            case OS_EXCEPTION_TYPE_FLOATING_POINT: return &ChainInfo::floatingPoint;
            case OS_EXCEPTION_TYPE_DECREMENTER: return &ChainInfo::decrementer;
            case OS_EXCEPTION_TYPE_SYSTEM_CALL: return &ChainInfo::systemCall;
            case OS_EXCEPTION_TYPE_TRACE: return &ChainInfo::trace;
            case OS_EXCEPTION_TYPE_PERFORMANCE_MONITOR: return &ChainInfo::performanceMonitor;
            case OS_EXCEPTION_TYPE_BREAKPOINT: return &ChainInfo::breakpoint;
            case OS_EXCEPTION_TYPE_SYSTEM_INTERRUPT: return &ChainInfo::systemInterrupt;
            case OS_EXCEPTION_TYPE_ICI: return &ChainInfo::ici;
            default: return nullptr;
        }
    }

    void Handler(OSExceptionType type, OSContext * interruptedContext, OSContext * callbackContext)
    {
        __OSSetCurrentUserContext(callbackContext);
        auto callback = GetCallback(type);
        if(callback)
        {
            if(callback(interruptedContext) == TRUE)
            {
                __OSSetAndLoadContext(interruptedContext);
                return;
            }
        }
        OSFatal(GetString(type));
    }

    void Initialize()
    {
        OSExceptionChainInfo cur;
        cur.callback = Handler;
        cur.stack = sStack + sizeof(sStack);
        cur.context = &sContext;
    
        OSExceptionType type;
        OSExceptionChainInfo* prev;
    
        type = OS_EXCEPTION_TYPE_SYSTEM_RESET;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_MACHINE_CHECK;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_DSI;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_ISI;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_EXTERNAL_INTERRUPT;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_ALIGNMENT;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_PROGRAM;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_FLOATING_POINT;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_DECREMENTER;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_SYSTEM_CALL;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_TRACE;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_PERFORMANCE_MONITOR;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_BREAKPOINT;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        type = OS_EXCEPTION_TYPE_SYSTEM_INTERRUPT;
        prev = GetChainInfo(type);
        __KernelSetUserModeExHandler(type, &cur, prev);
    
        // ICI is not set
    }

    void SetCallback(OSExceptionType type, OSExceptionCallbackFn function)
    {
        switch (type)
        {
            case OS_EXCEPTION_TYPE_SYSTEM_RESET: Callback::systemReset = function; break;
            case OS_EXCEPTION_TYPE_MACHINE_CHECK: Callback::machineCheck = function; break;
            case OS_EXCEPTION_TYPE_DSI: Callback::dsi = function; break;
            case OS_EXCEPTION_TYPE_ISI: Callback::isi = function; break;
            case OS_EXCEPTION_TYPE_EXTERNAL_INTERRUPT: Callback::externalInterrupt = function; break;
            case OS_EXCEPTION_TYPE_ALIGNMENT: Callback::alignment = function; break;
            case OS_EXCEPTION_TYPE_PROGRAM: Callback::program = function; break;
            case OS_EXCEPTION_TYPE_FLOATING_POINT: Callback::floatingPoint = function; break;
            case OS_EXCEPTION_TYPE_DECREMENTER: Callback::decrementer = function; break;
            case OS_EXCEPTION_TYPE_SYSTEM_CALL: Callback::systemCall = function; break;
            case OS_EXCEPTION_TYPE_TRACE: Callback::trace = function; break;
            case OS_EXCEPTION_TYPE_PERFORMANCE_MONITOR: Callback::performanceMonitor = function; break;
            case OS_EXCEPTION_TYPE_BREAKPOINT: Callback::breakpoint = function; break;
            case OS_EXCEPTION_TYPE_SYSTEM_INTERRUPT: Callback::systemInterrupt = function; break;
            case OS_EXCEPTION_TYPE_ICI: Callback::ici = function; break;
            default: break;
        }
    }
}