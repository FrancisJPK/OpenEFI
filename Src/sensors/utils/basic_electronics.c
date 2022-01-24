// custom:
#include "defines.h"
#include "user_defines.h"

int32_t convert_to_resistance(int32_t adcval)
{
#if defined(INPUT_PULL_UP)
	return (uint16_t)((R1 * ADC_MAX_VALUE) / adcval) / R1; //para resistencia pullUp dividir R1
#else
	return (int32_t)((R1 * ADC_MAX_VALUE) / adcval) - R1; //para resistencia pullDown multiplicar R1
#endif
}

int32_t convert_to_volt(int32_t in)
{
	return (int32_t)(in * Vref / ADC_MAX_VALUE);
}
