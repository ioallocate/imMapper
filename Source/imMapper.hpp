#pragma once
#include <windows.h>
#include "../AmdTools.hpp"

class imMapper
{
public:
    imMapper();
    ~imMapper();

    bool Initialize();
    bool IsInitialized() const;
    bool ReadMemory(uintptr_t address, void* buffer, size_t size);
    bool WriteMemory(uintptr_t address, const void* buffer, size_t size);
    bool ReadMsr(uint32_t index, uint64_t& out);
    bool WriteMsr(uint32_t index, uint64_t value);
    bool AllocPhysMem(uint16_t pages, Comms::PhysAlloc& out);

private:
    template<typename T>
    bool SendRequest(ULONG code, T& buf);
    void Cleanup();

    HANDLE deviceHandle;
    bool initialized;
};
