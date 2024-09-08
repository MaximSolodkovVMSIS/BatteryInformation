#include "header.h"

int main() {
    //printHeader();    //console output

    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point lastChargeCheck = startTime;
    std::chrono::seconds chargeDuration(0);

    while (true) {
        SYSTEM_POWER_STATUS status;
        if (GetSystemPowerStatus(&status)) {
            std::string powerSupply = getPowerSupply(status);
            std::string savingMode = getSavingMode(status);
            std::string batteryChemistry = getBatteryChemistry();

            std::string batteryLifeTime;
            if (status.BatteryLifeTime != -1) {
                int hours = status.BatteryLifeTime / 3600;
                int minutes = (status.BatteryLifeTime % 3600) / 60;
                batteryLifeTime = std::to_string(hours) + "h " + std::to_string(minutes) + "m";
            }
            else {
                batteryLifeTime = "N/A";
            }

            if (powerSupply == "mains") {
                chargeDuration += std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - lastChargeCheck);
                lastChargeCheck = std::chrono::steady_clock::now();
            }
            else {
                chargeDuration = std::chrono::seconds(0);
                lastChargeCheck = std::chrono::steady_clock::now();
            }

            std::string chargeTime = formatDuration(chargeDuration);
            updateValues(powerSupply, savingMode, static_cast<int>(status.BatteryLifePercent), batteryChemistry, chargeTime, batteryLifeTime);
        }
        /*else {
            std::cerr << "Failed to get system power status." << std::endl;
        }*/

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
