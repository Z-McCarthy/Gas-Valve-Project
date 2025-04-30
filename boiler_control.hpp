#ifndef BOILER_CONTROL_HPP
#define BOILER_CONTROL_HPP

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

// GPIO pin definitions 
#define PILOT_VALVE_PIN   BIT0  // P1.0
#define MAIN_VALVE_PIN    BIT1  // P1.1
#define SPARK_PIN         BIT2  // P1.2
#define FLAME_SENSOR_PIN  BIT3  // P1.3
#define THERMOSTAT_PIN    BIT4  // P1.4
#define POT_ADC_CHANNEL   0     // A0
#define THERM_ADC_CHANNEL 1     // A1

void clock_Init();
void delay_ms(uint32_t ms);

// Init functions
void pilotValve_Init();
void mainValve_Init();
void spark_Init();
void flame_Init();
void thermostat_Init();
void pot_Init();
void therm_Init();

// I/O functions
void pilotValve_Open();
void pilotValve_Close();
void mainValve_Open();
void mainValve_Close();
void spark_On();
void spark_Off();

bool flame_Detect();
bool thermostat_Read();
int16_t pot_Read();
int16_t therm_Read();

#endif
