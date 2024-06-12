#include "PowerMeter.h"

extern "C" {
#include <rapl.h>
}

#include <stdio.h>

static double initial_energy_pkg, initial_energy_pp0, initial_energy_pp1, initial_energy_dram;

int initialize_energy_monitor() {
    return init_rapl();
}

void terminate_energy_monitor() {
    terminate_rapl();
}

int start_energy_measurement() {
    if (get_total_energy_consumed(0, RAPL_PKG, &initial_energy_pkg) != 0 ||
        get_total_energy_consumed(0, RAPL_PP0, &initial_energy_pp0) != 0 ||
        get_total_energy_consumed(0, RAPL_PP1, &initial_energy_pp1) != 0 ||
        get_total_energy_consumed(0, RAPL_DRAM, &initial_energy_dram) != 0) {
        fprintf(stderr, "Failed to read initial energy from all domains\n");
        return 1;
    }
    return 0;
}

int end_energy_measurement(double *energy_pkg, double *energy_pp0, double *energy_pp1, double *energy_dram) {
    double current_energy_pkg, current_energy_pp0, current_energy_pp1, current_energy_dram;

    if (get_total_energy_consumed(0, RAPL_PKG, &current_energy_pkg) != 0 ||
        get_total_energy_consumed(0, RAPL_PP0, &current_energy_pp0) != 0 ||
        get_total_energy_consumed(0, RAPL_PP1, &current_energy_pp1) != 0 ||
        get_total_energy_consumed(0, RAPL_DRAM, &current_energy_dram) != 0) {
        fprintf(stderr, "Failed to read final energy from all domains\n");
        return 1;
    }

    *energy_pkg = current_energy_pkg - initial_energy_pkg;
    *energy_pp0 = current_energy_pp0 - initial_energy_pp0;
    *energy_pp1 = current_energy_pp1 - initial_energy_pp1;
    *energy_dram = current_energy_dram - initial_energy_dram;

    return 0;
}