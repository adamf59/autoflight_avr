#include "AF_HAL.h"
#include "serial_hal.h"
#include <avr/common.h>

// --- public methods ---

AF_SerialInterface::AF_SerialInterface(uint8_t* buffer, size_t buf_size,
      volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
      volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
      volatile uint8_t *udr,
      uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udre, uint8_t u2x): Stream(buffer, buf_size) {

    _ubrrh = ubrrh;
    _ubrrl = ubrrl;
    _ucsra = ucsra;
    _ucsrb = ucsrb;
    _udr = udr;
    _rxen = rxen;
    _txen = txen;
    _rxcie = rxcie;
    _udre = udre;
    _u2x = u2x;
}

AF_SerialInterface::~AF_SerialInterface() {
    close();
}

void AF_SerialInterface::open(uint32_t baud) {

    uint16_t baud_setting;
    bool use_u2x = true;
    
#if F_CPU == 16000000UL
    // hardcoded exception for compatibility with the bootloader shipped
    // with the Duemilanove and previous boards and the firmware on the 8U2
    // on the Uno and Mega 2560.
    if (baud == BAUDR_57600)  use_u2x = false; 
#endif

    if (use_u2x) {
        *_ucsra = 1 << _u2x;
        baud_setting = (F_CPU / 4 / baud - 1) / 2;
    } else {
        *_ucsra = 0;
        baud_setting = (F_CPU / 8 / baud - 1) / 2;
    }

    // assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
    *_ubrrh = baud_setting >> 8;
    *_ubrrl = baud_setting;

    // enable the receiver and transmitter
    *_ucsrb = (1 << _rxen) | (1 << _txen) | (1 << _rxcie) | (1 << _udre);
    
}

size_t AF_SerialInterface::close(void) {
    // disable the receiver and transmitter
    *_ucsrb &= ~((1 << _rxen) | (1 << _txen) | (1 << _rxcie) | (1 << _udre));
    // return the number of bytes in the buffer at the time of closing
    return flush();
}

size_t AF_SerialInterface::write(uint8_t byte) {

    // wait for the transmit buffer to be empty
    while (!(*_ucsra & (1 << _udre)));

    // write the byte to the transmit buffer
    *_udr = byte;

}

size_t AF_SerialInterface::write(const uint8_t* bytes, size_t size) {

    // write each byte in the buffer
    for (size_t i = 0; i < size; i++) {
        write(bytes[i]);
    }

    // return the number of bytes written
    return size;

}

inline void bufferput(uint8_t c, AF_SerialInterface* interface, uint8_t* buffer, size_t buf_size) {
    // interface->_readIdx;
}


// setup serial interfaces, based on the hardware

// define the stream buffer size in bytes
// if the available RAM is less than 1K, use a 64 byte buffer, otherwise use a 128 byte buffer
#if (RAMEND < 1000)
    #define SERIAL_RX_BUF_SIZE 64
#else
    #define SERIAL_RX_BUF_SIZE 128
    // todo: is this too much? with 4 serial ports opened, that's 512 bytes of RAM used by serial buffers...
#endif

namespace AF_HAL {

    namespace hwserial {

#if defined(EN_SERIAL_INTERFACE_0) && defined(UBRRH) && defined(UBRRL) && defined(USART_RX_vect) && defined(UDR)
        static uint8_t serial_rx_buf_0[SERIAL_RX_BUF_SIZE] = { 0 };
        static AF_SerialInterface SerialInterface0(serial_rx_buf_0, SERIAL_RX_BUF_SIZE, &UBRRH, &UBRRL, &UCSRA, &UCSRB, &UDR, RXEN, TXEN, RXCIE, UDRE, U2X);
        SIGNAL(USART_RX_vect) {
            uint8_t c = UDR;
            // SerialInterface0.put(c);
        }
#elif defined(EN_SERIAL_INTERFACE_0) && defined(UBRR0H) && defined(UBRR0L) && defined(USART_RX_vect) && defined(UDR0)
        static uint8_t serial_rx_buf_0[SERIAL_RX_BUF_SIZE] = { 0 };
        static AF_SerialInterface SerialInterface0(serial_rx_buf_0, SERIAL_RX_BUF_SIZE, &UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UDR0, RXEN0, TXEN0, RXCIE0, UDRE0, U2X0);
        SIGNAL(USART_RX_vect) {
            uint8_t c = UDR0;
            // SerialInterface0.put(c);
        }
#else
    #error "AutoFlight does not support this hardware (at least 1 serial interface is required)"
#endif
#if defined(EN_SERIAL_INTERFACE_1) && defined(UBRR1H) && defined(UBRR1L) && defined(USART2_RX_vect) && defined(UDR2)
        static uint8_t serial_rx_buf_1[SERIAL_RX_BUF_SIZE] = { 0 };
        static AF_SerialInterface SerialInterface1(serial_rx_buf_1, SERIAL_RX_BUF_SIZE, &UBRR1H, &UBRR1L, &UCSR1A, &UCSR1B, &UDR1, RXEN1, TXEN1, RXCIE1, UDRE1, U2X1);
        SIGNAL(USART2_RX_vect) {
            uint8_t c = UDR1;
            // SerialInterface1.put(c);
        }
#endif
#if defined(EN_SERIAL_INTERFACE_2) && defined(UBRR2H) && defined(UBRR2L) && defined(USART3_RX_vect) && defined(UDR2)
        static uint8_t serial_rx_buf_2[SERIAL_RX_BUF_SIZE] = { 0 };
        static AF_SerialInterface SerialInterface2(serial_rx_buf_2, SERIAL_RX_BUF_SIZE, &UBRR2H, &UBRR2L, &UCSR2A, &UCSR2B, &UDR2, RXEN2, TXEN2, RXCIE2, UDRE2, U2X2);
        SIGNAL(USART3_RX_vect) {
            uint8_t c = UDR2;
            // SerialInterface2.put(c);
        }
#endif
#if defined(EN_SERIAL_INTERFACE_3) && defined(UBRR3H) && defined(UBRR3L) && defined(USART4_RX_vect) && defined(UDR3)
        static uint8_t serial_rx_buf_3[SERIAL_RX_BUF_SIZE] = { 0 };
        static AF_SerialInterface SerialInterface3(serial_rx_buf_3, SERIAL_RX_BUF_SIZE, &UBRR3H, &UBRR3L, &UCSR3A, &UCSR3B, &UDR3, RXEN3, TXEN3, RXCIE3, UDRE3, U2X3);
        SIGNAL(USART3_RX_vect) {
            uint8_t c = UDR3;
            // SerialInterface3.put(c);
        }
#endif
    } // namespace hwserial

}   // namespace AF_HAL