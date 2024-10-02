#include "PowerMeter.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

unsigned long long read_energy_uj() {
    const std::string file_path = "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:0/energy_uj";
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "[ERROR] Failed to open energy file: " << file_path << std::endl;
        throw std::runtime_error("Failed to open energy file.");
    }

    unsigned long long energy_value;
    file >> energy_value;

    if (file.fail()) {
        std::cerr << "[ERROR] Failed to read energy value from file: " << file_path << std::endl;
        throw std::runtime_error("Failed to read energy value.");
    }

    file.close();
    return energy_value;
}


void PowerMeter::startMeasurement() {
    try {
        startEnergy = read_energy_uj();
    } catch (const std::runtime_error &e) {
        std::cerr << "[ERROR] Failed to start measurement: " << e.what() << std::endl;
        throw;
    }
}

unsigned long long PowerMeter::endMeasurement() {
    try {
        unsigned long long endEnergy = read_energy_uj();
        if (endEnergy >= startEnergy) {
            return endEnergy - startEnergy;
        } else {
            const unsigned long long MAX_ENERGY_UJ = 0xFFFFFFFFFFFFFFFF;
            unsigned long long energyDifference = (MAX_ENERGY_UJ - startEnergy) + endEnergy;
            std::cerr << "[WARNING] Counter overflow detected. Calculated energy difference with overflow handling." << std::endl;
            return energyDifference;
        }
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] Error ending measurement: " << e.what() << std::endl;
        return 0;
    }
}
