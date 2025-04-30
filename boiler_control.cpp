#include "boiler_control.hpp"

// === CLOCK ===
void clock_Init()
{
    CSCTL0_H = CSKEY_H;                      // Unlock CS registers
    CSCTL1 = DCOFSEL_4 | DCORSEL;            // Set DCO to 16 MHz
    CSCTL2 = SELM__DCOCLK | SELS__DCOCLK;    // MCLK and SMCLK = DCO
    CSCTL3 = DIVM__1 | DIVS__1;              // No division
    CSCTL0_H = 0;                            // Lock CS registers
}

// === DELAY ===
void delay_ms(uint32_t ms)
{
    __delay_cycles(16000 * ms);  // Assuming 16 MHz
}

// === INIT FUNCTIONS ===
void pilotValve_Init() { P1DIR |= PILOT_VALVE_PIN; P1OUT &= ~PILOT_VALVE_PIN; }
void mainValve_Init()  { P1DIR |= MAIN_VALVE_PIN;  P1OUT &= ~MAIN_VALVE_PIN;  }
void spark_Init()      { P1DIR |= SPARK_PIN;       P1OUT &= ~SPARK_PIN;       }
void flame_Init()      { P1DIR &= ~FLAME_SENSOR_PIN; P1REN |= FLAME_SENSOR_PIN; P1OUT |= FLAME_SENSOR_PIN; }
void thermostat_Init() { P1DIR &= ~THERMOSTAT_PIN;  P1REN |= THERMOSTAT_PIN;  P1OUT |= THERMOSTAT_PIN;  }
void pot_Init()        { /* ADC setup handled globally */ }
void therm_Init()      { /* ADC setup handled globally */ }

// === VALVE / SPARK CONTROL ===
void pilotValve_Open() { P1OUT |= PILOT_VALVE_PIN; }
void pilotValve_Close(){ P1OUT &= ~PILOT_VALVE_PIN; }

void mainValve_Open()  { P1OUT |= MAIN_VALVE_PIN; }
void mainValve_Close() { P1OUT &= ~MAIN_VALVE_PIN; }

void spark_On()        { P1OUT |= SPARK_PIN; }
void spark_Off()       { P1OUT &= ~SPARK_PIN; }

// === INPUT READ ===
bool flame_Detect()    { return (P1IN & FLAME_SENSOR_PIN); }
bool thermostat_Read() { return (P1IN & THERMOSTAT_PIN); }

// === ADC READ HELPERS ===
int16_t readADC(uint8_t channel)
{
    ADCCTL0 &= ADCENC;                       //Enabe ADC 
    ADCMCTL = ADCMEM0;                      //Enable ADC Control
    ADCCTL0 &= ~ADCENC;                     // Disable ADC
    ADCMCTL0 = channel;                     // Select channel
    ADCCTL0 |= ADCENC | ADCSC;              // Enable and start
    ADCCTL &= ~
    while (ADCCTL1 & ADCBUSY);              // Wait for finish
    return ADCMEM0;
}

int16_t pot_Read()     { return readADC(POT_ADC_CHANNEL); }
int16_t therm_Read()   { return readADC(THERM_ADC_CHANNEL); }


