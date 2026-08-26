#include "../Source/imMapper.hpp"
#include "../Source/Helpers/Logger.hpp"

static constexpr uint32_t MSR_LSTAR = 0xC0000082;
static constexpr uint32_t MSR_GS_BASE = 0xC0000101;
static constexpr uint32_t MSR_TSC_ADJUST = 0x0000003B;

int main()
{
    imMapper mapper;
    if (!mapper.Initialize()) {
        Console->Log("[poc_write_msr] failed to initialize");
        return 1;
    }

    uint64_t lstar = 0;
    if (mapper.ReadMsr(MSR_LSTAR, lstar)) {
        Console->Log("[LSTAR] read 0x%016llX", (unsigned long long)lstar);
        Console->Log("[LSTAR] write-back %s", mapper.WriteMsr(MSR_LSTAR, lstar) ? "OK" : "FAILED");
    }

    uint64_t gs = 0;
    if (mapper.ReadMsr(MSR_GS_BASE, gs)) {
        Console->Log("[GS_BASE] read 0x%016llX", (unsigned long long)gs);
        Console->Log("[GS_BASE] write-back %s", mapper.WriteMsr(MSR_GS_BASE, gs) ? "OK" : "FAILED");
    }

    uint64_t tsc_adj = 0;
    mapper.ReadMsr(MSR_TSC_ADJUST, tsc_adj);
    Console->Log("[TSC_ADJUST] original 0x%016llX", (unsigned long long)tsc_adj);
    Console->Log("[TSC_ADJUST] write 0 %s", mapper.WriteMsr(MSR_TSC_ADJUST, 0ULL) ? "OK" : "FAILED");
    Console->Log("[TSC_ADJUST] restore %s", mapper.WriteMsr(MSR_TSC_ADJUST, tsc_adj) ? "OK" : "FAILED");

    return 0;
}
