#include <avr/io.h>
#include "AF_HAL/system_hal.h"

void AF_HAL::init() {
    // initialize the system
    
};

namespace AF_HAL {
    namespace io {

        uint16_t aread(int pin) {
            ADMUX = (1 << REFS0) | (pin & 0x07); // select the pin and set reference voltage to AVCC
            ADCSRA |= (1 << ADSC);               // start the conversion
            while (ADCSRA & (1 << ADSC));        // wait for the conversion to finish
            return ADCW;                         // return the result
        };
        
        void awrite(int pin, int value) {
            // write to an analog pin
            // the atmega2560 does not have a DAC, so we cannot directly write an analog value
        };
        
        bool dread(uint8_t pin) {
            return (PINB & (1 << pin)) ? IO_LOGICAL_1 : IO_LOGICAL_0; // mask the PINB register to get the value of the pin
        };
        
        void dwrite(int pin, int value) {
#if defined(__AVR_ATmega2560__)
            // 


#elif defined(__AVR_ATmega328P__)
            // ATmega328P only has 8 digital pins, so we just manipulate PORTB
            if (value) {
                PORTB |= (1 << pin); // set the pin high
            } else {
                PORTB &= ~(1 << pin); // set the pin low
            }
#endif
        };
    
        void mode(int pin, int mode) {
            // set the mode of a pin
        };
    }
}