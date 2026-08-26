#include "imMapper.hpp"
#include <print>
#include <Psapi.h>
#include <setupapi.h>
#include <windows.h>

imMapper::imMapper()
    : deviceHandle(INVALID_HANDLE_VALUE)
    , initialized(false) {
}

imMapper::~imMapper()
{
    Cleanup();
}

bool imMapper::Initialize()
{
    GUID guid = GUID_DEVINTERFACE_AMDTOOLS;
    HDEVINFO device_info = SetupDiGetClassDevs(&guid, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    if (device_info == INVALID_HANDLE_VALUE)
        return false;

    SP_DEVICE_INTERFACE_DATA interface_data = { sizeof(SP_DEVICE_INTERFACE_DATA) };
    DWORD device_index = 0;
    bool device_found = false;

    while (SetupDiEnumDeviceInterfaces(device_info, nullptr, &guid, device_index, &interface_data)) {
        DWORD required_size = 0;
        SetupDiGetDeviceInterfaceDetail(device_info, &interface_data, nullptr, 0, &required_size, nullptr);

        if (required_size > 0) {
            std::vector<BYTE> buffer(required_size);
            PSP_DEVICE_INTERFACE_DETAIL_DATA interface_detail = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(buffer.data());
            interface_detail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

            if (SetupDiGetDeviceInterfaceDetail(device_info, &interface_data, interface_detail, required_size, nullptr, nullptr)) {
                deviceHandle = CreateFile(interface_detail->DevicePath,
                    GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    nullptr,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    nullptr);

                if (deviceHandle != INVALID_HANDLE_VALUE) {
                    std::println("[Integration Testing] Handle -> {}", deviceHandle);
                    device_found = true;
                    initialized = true;
                    break;
                }
                break;
            }
        }
        device_index++;
    }

    SetupDiDestroyDeviceInfoList(device_info);
    return device_found;
}

bool imMapper::IsInitialized() const
{
    return initialized;
}

template<typename T>
bool imMapper::SendRequest(ULONG code, T& buf)
{
    DWORD bytes = 0;
    return DeviceIoControl(deviceHandle, code, &buf, sizeof(T), &buf, sizeof(T), &bytes, nullptr) != 0;
}

bool imMapper::ReadMemory(uint64_t address, void* buffer, size_t size)
{
    Comms::RequestData buf{};
    buf.address = address;
    buf.size = static_cast<uint16_t>(size);
    if (!SendRequest(IOCTL_AMDTOOLS_PHYSICAL_READ, buf) || buf.status != 0)
        return false;
    memcpy(buffer, buf.data, size);
    return true;
}

bool imMapper::WriteMemory(uint64_t address, const void* buffer, size_t size)
{
    Comms::RequestData buf{};
    buf.address = address;
    buf.size = static_cast<uint16_t>(size);
    memcpy(buf.data, buffer, size);
    return SendRequest(IOCTL_AMDTOOLS_PHYSICAL_WRITE, buf) && buf.status == 0;
}

void imMapper::Cleanup()
{
    if (deviceHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(deviceHandle);
        deviceHandle = INVALID_HANDLE_VALUE;
    }
    initialized = false;
}

bool imMapper::ReadMsr(uint32_t index, uint64_t& out)
{
    Comms::MsrRequest buf{};
    buf.msrIndex = index;
    if (!SendRequest(IOCTL_AMDTOOLS_READ_MSR, buf))
        return false;
    out = buf.msrValue;
    return true;
}

bool imMapper::WriteMsr(uint32_t index, uint64_t value)
{
    Comms::MsrRequest buf{};
    buf.msrIndex = index;
    buf.msrValue = value;
    return SendRequest(IOCTL_AMDTOOLS_WRITE_MSR, buf);
}

bool imMapper::AllocPhysMem(uint16_t pages, Comms::PhysAlloc& out)
{
    Comms::AllocPhysBuffer buf{};
    buf.lowAddress = 0;
    buf.highAddress = 0xFFFFFFFFFFFFFFFFULL;
    buf.pageCount = pages;
    buf.cacheType = 1;
    if (!SendRequest(IOCTL_AMDTOOLS_ALLOC_PHYS, buf))
        return false;
    out = { buf.physicalAddress, buf.userVa, buf.mdlPtr, buf.kernelVa, buf.pageCount };
    return buf.physicalAddress != 0;
}
