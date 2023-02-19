#ifndef AF_GCS_H_
#define AF_GCS_H_

/// @file   AF_GCS.h
/// @brief  provides an interface for communicating with the ground control system

#define AF_GCS_COMPONENT_NAME_MAX_LEN 8
#define AF_GCS_MESSAGE_MAX_LEN 32

// GCS Component Identifiers
enum af_gcs_comp_group {
    AF_GCS_COMP_SYSTEM = 0,
    AF_GCS_COMP_TELEMETRY,
    AF_GCS_COMP_FLIGHT_SURFACE,
    AF_GCS_COMP_THRUSTER,
    AF_GCS_COMP_NAVIGATION,
    AF_GCS_COMP_AHRS,
    AF_GCS_COMP_SENSOR
};

enum af_gcs_priority {
    AF_GCS_PRIORITY_DEBUG = 0,
    AF_GCS_PRIORITY_INFO,
    AF_GCS_PRIORITY_WARN,
    AF_GCS_PRIORITY_ERROR,
    AF_GCS_PRIORITY_CRITICAL
};

typedef struct {
    const uint8_t START_BYTE = 0xFF;
    af_gcs_priority priority;
    af_gcs_comp_group group;
    uint8_t comp_name_len;
    char * comp_name;
    uint8_t message_len;
    char * message;
    

} AF_GCS_Message;

class AF_GCS {

    private:

            /// inbound queue, messages are prioritized
            // std::vector<AF_GCS_Message> gcs_ib_queue;

            /// singleton instance of the GCS
            static AF_GCS * _instance;
    
    public:
        
        AF_GCS() {
            // verify that this is the only instance of the GCS
            if (_instance != nullptr) {
                // TODO: give error
            }

            _instance = this;
        }

        static AF_GCS * get_singleton(void) {
            return _instance;
        }


};

namespace af_gcs {

    static AF_GCS* _instance = nullptr;

    /// initializes the GCS
    AF_GCS * init() {
        return new AF_GCS();
    }

};

/// convienence macro for building a GCS message
#define GCS_BUILD_MESSAGE(_group, _comp_name, _priority, _msg) AF_GCS_Message { .priority = _priority, .group = _group, .comp_name_len = strlen(_comp_name), .comp_name = _comp_name, .message_len = strlen(_msg), .message = _msg }

/// convienence macro for emitting a GCS message
#define GCS_EMIT(_group, _comp_name, _priority, _msg)
/// convienence macro for emitting a GCS message and waiting for an response
///
/// - scheduler will halt the running task until a response is received
///
/// _comp is the component to send the message from
/// _priority is the priority of the message
#define GCS_WAIT_FOR_ACK(_comp, _priority, _msg) ;

#endif