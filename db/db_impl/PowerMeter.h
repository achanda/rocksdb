#ifndef POWER_METER_H
#define POWER_METER_H

unsigned long long read_energy_uj();

class PowerMeter {
public:
    void startMeasurement();
    unsigned long long endMeasurement();

private:
    unsigned long long startEnergy;
};

#endif // POWER_METER_H
