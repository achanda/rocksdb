#ifndef POWER_METER_H
#define POWER_METER_H

#include <tuple>

std::tuple<unsigned long long, unsigned long long, unsigned long long> read_energy_uj();

class PowerMeter {
public:
    void startMeasurement();
    std::tuple<unsigned long long, unsigned long long, unsigned long long> endMeasurement();

private:
    unsigned long long startPackageEnergy;
    unsigned long long startCoreEnergy;
    unsigned long long startDramEnergy;
};

#endif // POWER_METER_H
