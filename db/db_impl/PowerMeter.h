#ifndef POWER_METER_H
#define POWER_METER_H

int read_energy_uj();

class PowerMeter {
public:
    void startMeasurement();
    int endMeasurement();

private:
    int startEnergy;
};

#endif // POWER_METER_H
