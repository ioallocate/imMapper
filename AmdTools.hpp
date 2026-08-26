#pragma once
#include <cstdint>
#include <guiddef.h>
#include <winioctl.h>
#include <initguid.h>

#ifndef CTL_CODE
#define CTL_CODE(DeviceType, Function, Method, Access) (                 \
((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)
#endif

#ifndef FILE_DEVICE_UNKNOWN
#define FILE_DEVICE_UNKNOWN 0x00000022
#endif

#ifndef METHOD_BUFFERED
#define METHOD_BUFFERED 0
#endif

#ifndef FILE_ANY_ACCESS
#define FILE_ANY_ACCESS 0
#endif

static const GUID GUID_DEVINTERFACE_AMDTOOLS = {
    0x1232175B, 0x1C34, 0x41FD,
    { 0xB1, 0x01, 0x34, 0x2D, 0x47, 0xB8, 0x28, 0xAC }
};

constexpr ULONG IOCTL_AMDTOOLS_PHYSICAL_READ = CTL_CODE(0xFFF0, 0xA07, METHOD_BUFFERED, FILE_ANY_ACCESS);
constexpr ULONG IOCTL_AMDTOOLS_PHYSICAL_WRITE = CTL_CODE(0xFFF0, 0xA08, METHOD_BUFFERED, FILE_ANY_ACCESS);
constexpr ULONG IOCTL_AMDTOOLS_VIRT_TO_PHYS = CTL_CODE(0xFFF0, 0xA34, METHOD_BUFFERED, FILE_ANY_ACCESS);
constexpr ULONG IOCTL_AMDTOOLS_READ_MSR = CTL_CODE(0xFFF0, 0xA01, METHOD_BUFFERED, FILE_ANY_ACCESS);
constexpr ULONG IOCTL_AMDTOOLS_WRITE_MSR = CTL_CODE(0xFFF0, 0xA02, METHOD_BUFFERED, FILE_ANY_ACCESS);
constexpr ULONG IOCTL_AMDTOOLS_ALLOC_PHYS = CTL_CODE(0xFFF0, 0xA2B, METHOD_BUFFERED, FILE_ANY_ACCESS);

namespace Comms {

    #pragma pack(push, 1)
    struct RequestData {
        uint64_t address;
        uint16_t size;
        uint16_t status;
        uint8_t data[256];
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct MsrRequest {
        uint32_t msrIndex;
        uint64_t msrValue;
        uint32_t flags;
        uint32_t maskParam;
    };
    #pragma pack(pop)

    struct AllocPhysBuffer {
        uint64_t lowAddress;
        uint64_t highAddress;
        uint64_t physicalAddress;
        uint64_t userVa;
        uint64_t mdlPtr;
        uint64_t kernelVa;
        uint32_t pageCount;
        uint32_t cacheType;
    };
    static_assert(sizeof(AllocPhysBuffer) == 56);

    struct PhysAlloc {
        uint64_t physicalAddress;
        uint64_t userVa;
        uint64_t mdlPtr;
        uint64_t kernelVa;
        uint32_t pageCount;
    };

}
