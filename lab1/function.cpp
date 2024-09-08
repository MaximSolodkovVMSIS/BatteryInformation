#include "header.h"

#pragma comment(lib, "PowrProf.lib")
#pragma comment(lib, "setupapi.lib")
#include <Poclass.h>
#include <Setupapi.h>

std::string getPowerSupply(SYSTEM_POWER_STATUS status) {
    if (status.ACLineStatus == 1) {
        return "mains";
    }
    else if (status.ACLineStatus == 0) {
        return "battery";
    }
    else {
        return "unknown";
    }
}

std::string getSavingMode(SYSTEM_POWER_STATUS status) {
    if (status.SystemStatusFlag) {
        return "on";
    }
    return "off";
}

std::string getBatteryChemistry() {
    std::string chemistry;

    HDEVINFO DeviceInfoSet;
    DeviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_BATTERY, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    SP_DEVICE_INTERFACE_DATA DeviceInterfaceData = { 0 };
    ZeroMemory(&DeviceInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
    DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    if (SetupDiEnumDeviceInterfaces(DeviceInfoSet, NULL, &GUID_DEVCLASS_BATTERY, 0, &DeviceInterfaceData)) {
        DWORD cbRequired = 0;

        SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, NULL, NULL, &cbRequired, NULL);
        PSP_DEVICE_INTERFACE_DETAIL_DATA pdidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR, cbRequired);
        pdidd->cbSize = sizeof(*pdidd);

        if (SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, pdidd, cbRequired, &cbRequired, NULL)) {
            HANDLE hBattery = CreateFile(pdidd->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            BATTERY_QUERY_INFORMATION BatteryQueryInformation = { 0 };
            DWORD bytesWait = 0;
            DWORD bytesReturned = 0;
            DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_TAG, &bytesWait, sizeof(bytesWait), &BatteryQueryInformation.BatteryTag,
                sizeof(BatteryQueryInformation.BatteryTag), &bytesReturned, NULL) && BatteryQueryInformation.BatteryTag;

            BATTERY_INFORMATION BatteryInfo = { 0 };
            BatteryQueryInformation.InformationLevel = BatteryInformation;

            if (DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_INFORMATION, &BatteryQueryInformation, sizeof(BatteryQueryInformation),
                &BatteryInfo, sizeof(BatteryInfo), &bytesReturned, NULL)) {
                for (int b = 0; b < sizeof(BatteryInfo.Chemistry); b++) {
                    if (BatteryInfo.Chemistry[b] != '\0') {
                        chemistry += BatteryInfo.Chemistry[b];
                    }
                }
            }

            CloseHandle(hBattery);
        }

        LocalFree(pdidd);
    }

    SetupDiDestroyDeviceInfoList(DeviceInfoSet);
    return chemistry;
}