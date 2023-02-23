#ifndef SERIAL_HAL_H_
#define SERIAL_HAL_H_

#include <system.h>
#include <util.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>


// temporary - enable serial interface 0
#define EN_SERIAL_INTERFACE_0

#define BAUDR_9600 9600
#define BAUDR_19200 19200
#define BAUDR_38400 38400
#define BAUDR_57600 57600
#define BAUDR_115200 115200

/// @brief a class for interfacing with hardware serial ports on AVR hardware
class AF_SerialInterface: public Stream {

    private:
        volatile uint8_t* _ubrrh; // pointer to the baud rate register high byte
        volatile uint8_t* _ubrrl; // pointer to the baud rate register low byte
        volatile uint8_t* _ucsra; // pointer to the control and status register A
        volatile uint8_t* _ucsrb; // pointer to the control and status register B
        volatile uint8_t* _udr;   // pointer to the data register
        uint8_t _rxen;            // the bit to enable the receiver
        uint8_t _txen;            // the bit to enable the transmitter
        uint8_t _rxcie;           // the bit to enable the receive complete interrupt
        uint8_t _udre;            // the bit to enable the data register empty interrupt
        uint8_t _u2x;             // the bit to enable double speed mode

    public:
    
        /// @brief opens a serial interface on the specified pins at the specified baud rate
        /// @param buffer the buffer to use for the stream
        /// @param ubrrh the pointer to the baud rate register high byte
        /// @param ubrrl the pointer to the baud rate register low byte
        /// @param ucsra the pointer to the control and status register A
        /// @param ucsrb the pointer to the control and status register B
        /// @param udr the pointer to the data register
        /// @param rxen the bit to enable the receiver
        /// @param txen the bit to enable the transmitter
        /// @param rxcie the bit to enable the receive complete interrupt
        /// @param udre the bit to enable the data register empty interrupt
        /// @param u2x the bit to enable double speed mode
        AF_SerialInterface(utilbuf::stream_buffer* buffer,
                           volatile uint8_t* ubrrh, volatile uint8_t* ubrrl,
                           volatile uint8_t* ucsra, volatile uint8_t* ucsrb,
                           volatile uint8_t* udr,
                           uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udre, uint8_t u2x);

        /// @brief see close()
        ~AF_SerialInterface();

        /// @brief opens the serial interface at the specified baud rate
        /// @param baud_rate the baud rate to use
        void open(uint32_t baud_rate);

        /// @brief closes the serial interface, releases the pins, and clears the stream buffer
        /// @return the number of bytes cleared from the stream buffer
        size_t close(void);

        virtual size_t write(uint8_t byte);
        virtual size_t write(const uint8_t* bytes, size_t size); 

           
};

namespace AF_HAL {

/// @brief namespace containing hardware serial interfaces for interacting with the serial ports on the AVR hardware
namespace hwserial {

#if defined(EN_SERIAL_INTERFACE_0) && (defined(UBRRH) || defined(UBRR0H))
    extern AF_SerialInterface SerialInterface0;
#endif
#if defined(EN_SERIAL_INTERFACE_1) && defined(UBRR1H)
    extern AF_SerialInterface SerialInterface1;
#endif
#if defined(EN_SERIAL_INTERFACE_2) && defined(UBRR2H)
    extern AF_SerialInterface SerialInterface2;
#endif
#if defined(EN_SERIAL_INTERFACE_3) && defined(UBRR3H)
    extern AF_SerialInterface SerialInterface3;
#endif

}

}

#endif // SERIAL_HAL_H_