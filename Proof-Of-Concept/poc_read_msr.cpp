#include "../Source/imMapper.hpp"
#include "../Source/Helpers/Logger.hpp"

static constexpr struct { uint32_t index; const char* name; } kMsrs[] = {
    { 0xC0000080, "IA32_EFER" },
    { 0xC0000081, "IA32_STAR" },
    { 0xC0000082, "IA32_LSTAR" },
    { 0xC0000083, "IA32_CSTAR" },
    { 0xC0000100, "FS_BASE" },
    { 0xC0000101, "GS_BASE" },
    { 0xC0000102, "KernelGSBase" },
    { 0x00000010, "IA32_TSC" },
    { 0x0000001B, "IA32_APIC_BASE" },
    { 0x0000008B, "IA32_BIOS_SIGN" },
    { 0x000000FE, "IA32_MTRRCAP" },
    { 0x00000174, "IA32_SYSENTER_CS" },
    { 0x00000175, "IA32_SYSENTER_SP" },
    { 0x00000176, "IA32_SYSENTER_IP" },
};

int main()
{
    imMapper mapper;
    if (!mapper.Initialize()) {
        Console->Log("[poc_read_msr] failed to initialize");
        return 1;
    }

    for (const auto& msr : kMsrs) {
        uint64_t value = 0;
        if (mapper.ReadMsr(msr.index, value))
            Console->Log("[0x%08X] %s = 0x%016llX", msr.index, msr.name, (unsigned long long)value);
        else
            Console->Log("[0x%08X] %s = <failed>", msr.index, msr.name);
    }

    return 0;
}
