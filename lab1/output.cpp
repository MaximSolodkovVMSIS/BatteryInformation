#include "header.h"

//commented lines for console output

void setCursorPosition(int x, int y) {
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearLine(int x, int y, int length) {
    setCursorPosition(x, y);
    /*for (int i = 0; i < length; ++i) {
        std::cout << " ";
    }*/
}

void updateValues(const std::string& powerSupply, const std::string& savingMode, int percentage,
    const std::string& batteryChemistry, const std::string& chargeTime, const std::string& batteryLifeTime) {

    //clearLine(14, 0, 20);
    //setCursorPosition(14, 0); // Power supply
    //std::cout << powerSupply << std::endl;

    //clearLine(13, 1, 20);
    //setCursorPosition(13, 1); // Saving mode
    //std::cout << savingMode << std::endl;

    //clearLine(12, 2, 20);
    //setCursorPosition(12, 2); // Percentage
    //std::cout << percentage << "%" << std::endl;

    //clearLine(19, 3, 20);
    //setCursorPosition(19, 3); // Chemistry
    //std::cout << batteryChemistry << std::flush;

    //clearLine(13, 4, 20);
    //setCursorPosition(13, 4); // Charge time
    //std::cout << chargeTime << std::flush;

    //clearLine(19, 5, 20);
    //setCursorPosition(19, 5); // Battery Life Time
    //std::cout << batteryLifeTime << std::flush;

    std::ofstream outFile("D:/AStydy/IiYVMqt/battery_info.txt", std::ios::out | std::ios::trunc);
    if (outFile.is_open()) {
        outFile << "Power supply: " << powerSupply << std::endl;
        outFile << "Saving mode: " << savingMode << std::endl;
        outFile << "Percentage: " << percentage << "%" << std::endl;
        outFile << "Battery Chemistry: " << batteryChemistry << std::endl;
        outFile << "Charge Time: " << chargeTime << std::endl;
        outFile << "Battery Life Time: " << batteryLifeTime << std::endl;
        outFile.close();
    }
    /*else {
        std::cerr << "Failed to open file for writing." << std::endl;
    }*/
}

/*
void printHeader() {
    std::cout << "Power supply: " << std::endl
        << "Saving mode: " << std::endl
        << "Percentage: " << std::endl
        << "Battery Chemistry: " << std::endl
        << "Charge Time: " << std::endl
        << "Battery life time: " << std::endl;
}
*/

std::string formatDuration(std::chrono::seconds duration) {
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration % std::chrono::hours(1));
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration % std::chrono::minutes(1));
    std::ostringstream oss;
    if (hours.count() > 0) {
        oss << hours.count() << "h ";
    }
    if (minutes.count() > 0 || hours.count() > 0) {
        oss << minutes.count() << "m ";
    }
    oss << seconds.count() << "s";
    return oss.str();
}