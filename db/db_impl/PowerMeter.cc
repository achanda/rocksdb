#include "PowerMeter.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

std::tuple<unsigned long long, unsigned long long, unsigned long long> read_energy_uj() {
    const std::string package_file_path = "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:0/energy_uj";
    const std::string core_file_path = "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:0:0/energy_uj";
    const std::string dram_file_path = "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:0:1/energy_uj";

    std::ifstream package_file(package_file_path);
    std::ifstream core_file(core_file_path);
    std::ifstream dram_file(dram_file_path);

    if (!package_file.is_open()) {
        std::cerr << "[ERROR] Failed to open package energy file: " << package_file_path << std::endl;
        throw std::runtime_error("Failed to open package energy file.");
    }
    
    if (!core_file.is_open()) {
        std::cerr << "[ERROR] Failed to open core energy file: " << core_file_path << std::endl;
        throw std::runtime_error("Failed to open core energy file.");
    }
    
    if (!dram_file.is_open()) {
        std::cerr << "[ERROR] Failed to open dram energy file: " << dram_file_path << std::endl;
        throw std::runtime_error("Failed to open dram energy file.");
    }

    unsigned long long package_energy_value;
    unsigned long long core_energy_value;
    unsigned long long dram_energy_value;

    package_file >> package_energy_value;

    if (package_file.fail()) {
        std::cerr << "[ERROR] Failed to read energy value from package file: " << package_file_path << std::endl;
        throw std::runtime_error("Failed to read package energy value.");
    }
    
    core_file >> core_energy_value;
    if (core_file.fail()) {
        std::cerr << "[ERROR] Failed to read energy value from core file: " << core_file_path << std::endl;
        throw std::runtime_error("Failed to read core energy value.");
    }
    
    dram_file >> dram_energy_value;
    if (dram_file.fail()) {
        std::cerr << "[ERROR] Failed to read energy value from dram file: " << dram_file_path << std::endl;
        throw std::runtime_error("Failed to read dram energy value.");
    }

    package_file.close();
    core_file.close();
    dram_file.close();
    return {package_energy_value, core_energy_value, dram_energy_value};
}


void PowerMeter::startMeasurement() {
    try {
        std::tie(startPackageEnergy, startCoreEnergy, startDramEnergy) = read_energy_uj();
    } catch (const std::runtime_error &e) {
        std::cerr << "[ERROR] Failed to start measurement: " << e.what() << std::endl;
        throw;
    }
}

std::tuple<unsigned long long, unsigned long long, unsigned long long> PowerMeter::endMeasurement() {
    try {
        auto [endPackageEnergy, endCoreEnergy, endDramEnergy] = read_energy_uj();
        
        // Helper function to calculate energy difference with overflow handling
        auto calculateEnergyDiff = [](unsigned long long start, unsigned long long end, const char* counterName) -> unsigned long long {
            if (end > start) {
                return end - start;
            } else if (end < start) {
                const unsigned long long MAX_ENERGY_UJ = 0xFFFFFFFFFFFFFFFF;
                std::cerr << "[WARNING] " << counterName << " counter overflow detected. Calculated energy difference with overflow handling." << std::endl;
                return (MAX_ENERGY_UJ - start) + end;
            } else {
                std::cerr << "[WARNING] ZERO " << counterName << " energy difference. start: " << start << " end: " << end << std::endl;
                return 0;
            }
        };
        
        // Calculate all three energy differences
        unsigned long long packageDiff = calculateEnergyDiff(startPackageEnergy, endPackageEnergy, "Package");
        unsigned long long coreDiff = calculateEnergyDiff(startCoreEnergy, endCoreEnergy, "Core");
        unsigned long long dramDiff = calculateEnergyDiff(startDramEnergy, endDramEnergy, "DRAM");
        
        return {packageDiff, coreDiff, dramDiff};
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] Error ending measurement: " << e.what() << std::endl;
        return {0, 0, 0};
    }
}
