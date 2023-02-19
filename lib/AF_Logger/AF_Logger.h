#ifndef AF_LOGGER_H_
#define AF_LOGGER_H_

/// @file   AF_Logger.h
/// @brief  provides an interface for routing messages to different
///         loggers, such as the GCS, SD card, or serial port

// reserved logger destinations
#define AF_LOG_TO_TELMETRY (1 << 0)
#define AF_LOG_TO_SD (1 << 1)
#define AF_LOG_TO_SERIAL (1 << 2)


enum AF_Logger_Stream: uint8_t {
    AF_LOGGER_STREAM_ALL = 0,
    AF_LOGGER_STREAM_TELMETRY,
    AF_LOGGER_STREAM_SD,
    AF_LOGGER_STREAM_SERIAL
};


class AF_Logger {

    private:

        /// singleton instance of the logger
        static AF_Logger * _instance;


    public:

        AF_Logger() {
            // verify that this is the only instance of the logger
            if (_instance != nullptr) {

            }
        }

        /// @brief  logs a message to the specified stream
        /// @param  stream  the stream to log to
        /// @param  message the message to log
        void log(uint8_t stream, const char * message);
};

/// macro for logging a message to all (open) streams, including unreserved streams.
/// @param message the message to log
#define AF_LOG_TO_ALL(message) AF_Logger::log(AF_LOGGER_STREAM_ALL, message)


#endif