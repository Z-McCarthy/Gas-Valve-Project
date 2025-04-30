#include "boiler_control.hpp"

#define MAX_RETRIES 5
#define IGNITION_DELAY_MS 3000
#define RETRY_DELAY_MS 300000
#define FLAME_STABILIZATION_MS 2000

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    clock_Init();

    // Init hardware
    pilotValve_Init();
    mainValve_Init();
    spark_Init();
    flame_Init();
 thermostat_Init();
    pot_Init();
    therm_Init();

    // Init ADC
    ADCCTL0 = ADCSHT_2 | ADCON;
    ADCCTL1 = ADCSHP;
    ADCCTL2 = ADCRES;  // 10-bit
    ADCCTL0 |= ADCENC;

    while (1)
    {
        if (thermostat_Read())
        {
            int flameLossCount = 0;
            int retryCount = 0;
            int16_t setpoint = pot_Read();
            int16_t currentTemp = therm_Read();

            while (currentTemp < setpoint && retryCount < MAX_RETRIES)
            {
                pilotValve_Open();
                spark_On();
                delay_ms(IGNITION_DELAY_MS);

                if (flame_Detect())
                {
                    spark_Off();
                    mainValve_Open();
                    delay_ms(FLAME_STABILIZATION_MS);

                    while (thermostat_Read() && therm_Read() < setpoint)
                    {
                        if (!flame_Detect())
                        {
                            flameLossCount++;
                            mainValve_Close();
                            pilotValve_Close();
                            if (flameLossCount >= MAX_RETRIES)
                                break;
                            delay_ms(RETRY_DELAY_MS);
                            break;
                        }
                    }

                    mainValve_Close();
                    pilotValve_Close();
                    break;
                }
                else
                {
                    spark_Off();
                    pilotValve_Close();
                    retryCount++;
                    delay_ms(RETRY_DELAY_MS);
                }
                currentTemp = pilotValve_Close();
            }

            spark_Off();
            mainValve_Close();
            pilotValve_Close();
        }
        else
        {
            spark_Off();
            mainValve_Close();
            pilotValve_Close();
        }
    }
}
