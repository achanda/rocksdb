#ifndef POWER_METER_H
#define POWER_METER_H


// Initialize the RAPL library
int initialize_energy_monitor();

// Terminate the RAPL library
void terminate_energy_monitor();

// Start measuring energy
int start_energy_measurement();

// End measuring energy and get the consumed values
int end_energy_measurement(double *energy_pkg, double *energy_pp0, double *energy_pp1, double *energy_dram);

#endif // POWER_METER_H