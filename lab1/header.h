#pragma once

#include <Windows.h>
#include <iostream>
#include <powrprof.h>
#include <thread>   
#include "conio.h"
#include <devguid.h>
#include <chrono>
#include <sstream>
#include <fstream>

//logic
std::string getPowerSupply(SYSTEM_POWER_STATUS status);
std::string getSavingMode(SYSTEM_POWER_STATUS status);
std::string getBatteryChemistry();

//output
void updateValues(const std::string& powerSupply, const std::string& savingMode, int percentage,
    const std::string& batteryChemistry, const std::string& chargeTime, const std::string& batteryLifeTime);
void printHeader();
std::string formatDuration(std::chrono::seconds duration);