#include "PowerMeter.h"

#include <iostream>

unsigned long long read_energy_uj() {
    // File path
    const char *file_path = "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:0/energy_uj";
    FILE *file;
    unsigned long long energy_value;

    // Open the file for reading
    file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Failed to open file");
        throw std::runtime_error("Failed to open energy file.");
    }

    // Read the integer value from the file
    if (fscanf(file, "%llu", &energy_value) != 1) {
        perror("Failed to read integer from file");
        fclose(file);
        throw std::runtime_error("Failed to read energy value.");
    }

    // Close the file
    fclose(file);

    // Return the integer value
    return energy_value;
}


void PowerMeter::startMeasurement() {
    try {
        startEnergy = read_energy_uj();
    } catch (const std::runtime_error &e) {
        std::cerr << "Failed to start measurement: " << e.what() << std::endl;
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
            std::cerr << "Warning: Counter overflow detected. Calculated energy difference with overflow handling." << std::endl;
            return energyDifference;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error ending measurement: " << e.what() << std::endl;
        return 0;
    }
}
