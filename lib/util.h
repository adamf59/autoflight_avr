#ifndef UTIL_H_
#define UTIL_H_

#include "system.h"
#include <stdint.h>
#include <stdlib.h>

namespace utilbuf {

    static uint16_t bytes_dropped = 0;

    struct stream_buffer {
        uint8_t* buffer = nullptr;
        size_t size = 4;
        size_t read_idx = 0;
        size_t write_idx = 0;
    } ;

    /// @brief puts a byte into a stream buffer
    static inline void bufferput(uint8_t c, stream_buffer* sb) {
        // complete cycle for cyclic buffer
        if ( sb->write_idx == sb->size ) sb->write_idx = 0;
        // if the write head is at the read head, the buffer is full, so increment the bytes dropped counter.
        if ( sb->write_idx == sb->read_idx ) {
            bytes_dropped += 1;
            return;
        };
        // put the new byte at the write head
        sb->buffer[sb->write_idx] = c;
        // advance the write head
        sb->write_idx += 1;
    }

    static inline uint8_t bufferget(stream_buffer* sb) {
        // complete cycle for cyclic buffer
        if ( sb->read_idx == sb->size ) sb->read_idx = 0;
        // if the read head is at the write head, the buffer is empty, so return 0.
        if ( sb->read_idx == sb->write_idx ) return 0;
        // get the byte at the read head
        uint8_t c = sb->buffer[sb->read_idx];
        // advance the read head
        sb->read_idx += 1;
        return c;
    }

}

/// @brief a simple stream interface, with a buffer that can be read from and written to.
class Stream {

    public:
        /// @brief creates a stream with a buffer of size n
        /// @param size the size of the stream buffer
        Stream(utilbuf::stream_buffer* buffer) : _buffer(buffer) {};

        /// @brief destructor
        virtual ~Stream();

        /// @brief gets the next byte in the stream buffer and pops it
        /// @return the next byte in the stream buffer
        uint8_t read() { return utilbuf::bufferget(_buffer); };

        /// @brief gets the next n bytes in the stream buffer and pops them.
        ///        if n is greater than the number of bytes in the stream buffer,
        ///        the remaining bytes will be nullptr.
        /// @param n how many bytes to read from the stream buffer
        /// @param ap pointer to the start of the array to store the bytes in
        /// @return the next n bytes in the stream buffer
        void read(size_t n, uint8_t* ap = nullptr) {
            for (size_t i = 0; i < n; i++) {
                ap[i] = utilbuf::bufferget(_buffer);
            }
        }

        /// @brief gets the next byte in the stream buffer without removing it
        /// @return the next byte in the stream buffer
        uint8_t peek();

        /// @brief clears the stream buffer
        /// @return the number of bytes cleared from the stream buffer
        size_t flush();

        /// @brief writes a byte to the stream
        /// @param byte the byte to write to the stream
        /// @return 1
        virtual size_t write(uint8_t byte) = 0;

        /// @brief  writes many bytes to the stream
        /// @param bytes pointer to the start of the bytes to write to the stream
        /// @param size how many bytes to write to the stream
        /// @return the number of bytes written to the stream (arg: size)
        virtual size_t write(const uint8_t* bytes, size_t size) = 0;

        /// @brief gets the number of bytes in the stream buffer
        /// @return the number of bytes in the stream buffer
        size_t available();

        /// @brief checks if the stream buffer is empty
        /// @return 0 if the stream buffer is not empty, 1 if it is empty
        bool empty() { return available() == 0; }

        /// @brief  gets the size of the stream buffer
        /// @return the size of the stream buffer
        size_t size() const { return _buffer->size; }

    protected:
        /// @brief  the buffer
        utilbuf::stream_buffer* _buffer;
    
};

#endif // UTIL_H_