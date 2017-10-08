// MBED reference code for the ST Micro STEVAL-MKI124V1 header board
// This board has: LPS331 pressure/temperature sensor, L3GD20 gyroscope and LSM303DLHC magnetometer/accelerometer
// Code accesses each of the 3 MEMS sensors and calculates pressure, temp, heading, tilt, roll and angular velocity
// Code is not optimized for efficienecy but instead for clarity of how you use the sensors
// ST application note AN3192 was key in developing the tilt-corrected compass
// Developed on an LPC1768
// By Liam Goudge. March 2014

#define LSM303_on

#include "mbed.h"
#include "MKI124V1.h"
#include "math.h"
#include "mlx90614.h"

float get_temperature(void);

char readByte(char address, char reg);

void writeByte(char address, char reg,char value);

void initSensors (void);

void LSM303 (SensorState_t * state);

void calc_avrg_ac(Result_avrg* result,int samples);

void calc_avrg_or(Result_avrg* result,int samples);

void calc_diff(Result_avrg* r1, Result_avrg* r2);