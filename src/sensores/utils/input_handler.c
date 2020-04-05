/* Esto se va a encargar de manejar el CD74HC4067 para obtener los diferentes sensores */
#ifndef INPUT_HANDLER
#define INPUT_HANDLER
#include <stdint.h>
// libopencm3:
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
// custom:
#include "./ema_low_pass.c"
struct input_handler{
	/* data */
	struct EMALowPass values[16]; // valores pasados por EMA Low Pass
} inputs;

/** devuelve input selecionada pasada por filtro EMA Low Pass
  * @param pin entrada analogica a leer
*/
uint16_t get_input(uint8_t);
/** devuelve data RAW del adc
*/
uint16_t get_adc_data(uint8_t);
/** inicia el ADC y los trigers por tiempo
*/
void input_setup(void);

uint16_t get_input(uint8_t pin){
	/*
   el sw para cambiar I/O del 74HC4067 para elegir el canal
   switch (pin){
    case 0:
		
        break;
    
    default:
        break;
    } */

	if(pin < 16){
		inputs.values[pin].actualValue = get_adc_data(7);
        inputs.values[pin] = EMALowPassFilter(inputs.values[0]);
		return inputs.values[pin].lastValue;
	}
}

void input_setup(){
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_set_mode(GPIOA,
				  GPIO_MODE_INPUT,
				  GPIO_CNF_INPUT_ANALOG, // Analog mode
				  GPIO7);
}

static void adc_setup(void){
	input_setup();
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_ADC1EN);
	adc_power_off(ADC1);
	rcc_peripheral_reset(&RCC_APB2RSTR, RCC_APB2RSTR_ADC1RST);
	rcc_peripheral_clear_reset(&RCC_APB2RSTR, RCC_APB2RSTR_ADC1RST);
	rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV6); // Set. 12MHz, Max. 14MHz
	adc_set_dual_mode(ADC_CR1_DUALMOD_IND);		// Independent mode
	adc_disable_scan_mode(ADC1);
	adc_set_right_aligned(ADC1);
	adc_set_single_conversion_mode(ADC1);
	adc_set_sample_time(ADC1, ADC_CHANNEL_TEMP, ADC_SMPR_SMP_239DOT5CYC);
	adc_enable_temperature_sensor();
	adc_power_on(ADC1);
	adc_reset_calibration(ADC1);
	adc_calibrate_async(ADC1);
	while (adc_is_calibrating(ADC1));
}

uint16_t get_adc_data(uint8_t channel){
	adc_set_sample_time(ADC1, channel, ADC_SMPR_SMP_239DOT5CYC);
	adc_set_regular_sequence(ADC1, 1, &channel);
	adc_start_conversion_direct(ADC1);
	while (!adc_eoc(ADC1));
	return (adc_read_regular(ADC1) * 3300 / 4095 );
}

#endif
