#include "../Source/imMapper.hpp"
#include "../Source/Helpers/Logger.hpp"
#include <cstdint>
#include <cstring>

static constexpr uint64_t PATTERN_64 = 0xDEADBEEFCAFEBABEULL;
static constexpr uint8_t PATTERN_AA = 0xAA;
static constexpr uint8_t PATTERN_55 = 0x55;

int main()
{
    imMapper mapper;
    if (!mapper.Initialize()) {
        Console->Log("[poc_write_kernel_mem] failed to initialize");
        return 1;
    }

    Comms::PhysAlloc alloc{};
    if (!mapper.AllocPhysMem(1, alloc)) {
        Console->Log("[poc_write_kernel_mem] AllocPhysMem FAILED");
        return 1;
    }

    Console->Log("[AllocPhysMem] physAddr=0x%016llX userVa=0x%016llX",
        (unsigned long long)alloc.physicalAddress, (unsigned long long)alloc.userVa);

    uint64_t phys = alloc.physicalAddress;

    {
        uint64_t wval = PATTERN_64, rval = 0;
        bool wrote = mapper.WriteMemory(phys, &wval, sizeof(wval));
        bool read = wrote && mapper.ReadMemory(phys, &rval, sizeof(rval));
        Console->Log("[8-byte 0x%016llX] write=%s verify=%s",
            (unsigned long long)wval, wrote ? "OK" : "FAIL",
            (read && rval == wval) ? "PASS" : "FAIL");
    }

    {
        uint8_t wbuf[64], rbuf[64]{};
        memset(wbuf, PATTERN_AA, sizeof(wbuf));
        bool wrote = mapper.WriteMemory(phys, wbuf, sizeof(wbuf));
        bool read = wrote && mapper.ReadMemory(phys, rbuf, sizeof(rbuf));
        Console->Log("[64-byte 0xAA] write=%s verify=%s",
            wrote ? "OK" : "FAIL",
            (read && memcmp(wbuf, rbuf, sizeof(wbuf)) == 0) ? "PASS" : "FAIL");
    }

    {
        uint8_t wbuf[64], rbuf[64]{};
        memset(wbuf, PATTERN_55, sizeof(wbuf));
        bool wrote = mapper.WriteMemory(phys, wbuf, sizeof(wbuf));
        bool read = wrote && mapper.ReadMemory(phys, rbuf, sizeof(rbuf));
        Console->Log("[64-byte 0x55] write=%s verify=%s",
            wrote ? "OK" : "FAIL",
            (read && memcmp(wbuf, rbuf, sizeof(wbuf)) == 0) ? "PASS" : "FAIL");
    }

    {
        uint8_t zeros[64]{}, rbuf[64];
        memset(rbuf, 0xFF, sizeof(rbuf));
        bool wrote = mapper.WriteMemory(phys, zeros, sizeof(zeros));
        bool read = wrote && mapper.ReadMemory(phys, rbuf, sizeof(rbuf));
        Console->Log("[64-byte zero] write=%s verify=%s",
            wrote ? "OK" : "FAIL",
            (read && memcmp(zeros, rbuf, sizeof(zeros)) == 0) ? "PASS" : "FAIL");
    }

    return 0;
}
