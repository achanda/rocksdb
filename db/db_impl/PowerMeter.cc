#include "PowerMeter.h"

#include <iostream>

int read_energy_uj() {
    // File path
    const char *file_path = "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:0/energy_uj";
    FILE *file;
    int energy_value;

    // Open the file for reading
    file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return -1; // Error code
    }

    // Read the integer value from the file
    if (fscanf(file, "%d", &energy_value) != 1) {
        perror("Failed to read integer from file");
        fclose(file);
        return -1; // Error code
    }

    // Close the file
    fclose(file);

    // Return the integer value
    return energy_value;
}


void PowerMeter::startMeasurement() {
    startEnergy = read_energy_uj();
}

int PowerMeter::endMeasurement() {
    int endEnergy = read_energy_uj();
    if (endEnergy >= startEnergy) {
        return endEnergy - startEnergy;
    } else {
        std::cerr << "Error: End measurement is less than start measurement." << std::endl;
        return -1;
    }
}
