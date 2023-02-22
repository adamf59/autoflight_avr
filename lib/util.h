#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>
#include <stdlib.h>

/// @brief a simple stream interface, with a buffer that can be read from and written to.
class Stream {

    private:
        /// @brief  the size of the stream buffer
        const size_t _size = 0;

    public:
        /// @brief creates a stream with a buffer of size n
        /// @param size the size of the stream buffer
        Stream(uint8_t* buffer, size_t size);

        /// @brief destructor
        virtual ~Stream();

        /// @brief gets the next byte in the stream buffer and pops it
        /// @return the next byte in the stream buffer
        uint8_t read();

        /// @brief gets the next n bytes in the stream buffer and pops them.
        ///        if n is greater than the number of bytes in the stream buffer,
        ///        the remaining bytes will be nullptr.
        /// @param n how many bytes to read from the stream buffer
        /// @return the next n bytes in the stream buffer
        uint8_t* read(size_t n);

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
        size_t size() { return _size; }

    protected:
        /// @brief  the stream buffer
        uint8_t* _buffer = nullptr;

        /// @brief  the read index of the stream buffer
        size_t _readIdx = 0;

        /// @brief  the write index of the stream buffer
        size_t _writeIdx = 0;
    
};

#endif // UTIL_H_