#include "../include/tps.hpp"
#include "dtc_codes.h"

int32_t TPS::last_value = 0;

int32_t TPS::get_value(int32_t filt_input)
{
    int32_t _t = convert_to_volt(filt_input) * 1334;
    if (_t > TPS_MAX_A || _t < TPS_MIN_A)
        return 0;
    TPS::last_value = (int32_t)TPS_CALC_A(_t);
    return (int32_t)last_value;
}

int32_t TPS::get_calibrate_value(int32_t filt_input)
{
    int32_t _t = convert_to_volt(filt_input) * 1334;
    if (_t > TPS_MAX_A || _t < TPS_MIN_A)
        return 0;
    return (int32_t)TPS_CALC_A(_t) * 1000;
}

uint8_t *TPS::dtc()
{

    if (TPS::last_value > TPS_MAX_A)
        return NEW_DTC DTC_TPS_OUT_OF_RANGE;
    if (TPS::last_value < TPS_MIN_A)
        return NEW_DTC DTC_TPS_OUT_OF_RANGE;
    return 0;
}

uint8_t TPS::dtc(int32_t in1, int32_t in2)
{
    //TODO: laburar x1
    return 0;
}
int32_t TPS::get_value(int32_t filt_input, int32_t filt_input2)
{
    //TODO: laburar x2
    return 0;
}

int32_t TPS::get_calibrate_value(int32_t filt_input, int32_t filt_input2)
{
    //TODO: laburar x3
    return 0;
}