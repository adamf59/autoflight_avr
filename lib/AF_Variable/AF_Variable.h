#ifndef AF_VARIABLE_H_
#define AF_VARIABLE_H_

/// @file   AF_Variable.h
/// @brief  standardizes types for AutoFlight, and exposes an interface for building
///         on top of an AP_Variable, such as auto-storing the variable in EEPROM.
///         Exposes declared variables to the GCS in packets.

#define NDEBUG

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/// max length of an `AP_Variable` identifier
#define AF_VAR_MAX_IDFR_LEN 16

/// macro for typedefining instances of the AF_VarScalar template.
/// _t is the base type
/// _name will be the name of the typedef, with AF_ prepended
/// _vt is the enum af_var_type type
#define AF_DEF_VARTYPE_SCALAR(_t, _name, _vt)  typedef AF_Var_Scalar<_t, _vt> AF_ ## _name;

// define variable flags

#define AF_VAR_FLAG_READABLE_BY_GCS (1 << 0)
#define AF_VAR_FLAG_WRITABLE_BY_GCS (1 << 1)
#define AF_VAR_FLAG_EEPROM_STORED (1 << 2)
#define AF_VAR_FLAG_BLACKBOX_LOGGED (1 << 3)

// define variable types
// (new types should be added to the end of the list, old types should not be removed!)
// these are used to encode types into eeprom storage and for some packets
enum af_var_type {
    AF_VAR_BOOL = 0,
    AF_VAR_INT8,
    AF_VAR_UINT8,
    AF_VAR_INT16,
    AF_VAR_UINT16,
    AF_VAR_INT32,
    AF_VAR_UINT32,
    AF_VAR_FLOAT
};

typedef const char * af_var_idfr_t;

/// linked-list style node for storing AF_Variable instances in AF_Variable_Storage
struct AF_Variable_Node {
    AF_Variable_Node* next;
    AF_Variable* var;
};

/// stores AF_Variable instances and provides access to them 
class AF_Variable_Storage {

    public:
        
        /// get the number of variables
        uint16_t get_num_variables(void) const { return _num_variables; }

        /// @brief get a variable by its identifier
        /// @param idfr the identifier of the variable
        /// @return pointer to the variable, or nullptr if the variable does not exist
        AF_Variable* get_variable(af_var_idfr_t idfr) {
            /// search the linked list for the variable
            AF_Variable_Node* node = _head;
            while (node != nullptr) {
                if (strcmp(node->var->get_idfr(), idfr) == 0) {
                    return node->var;
                }
                node = node->next;
            }
            
            return nullptr;
        }

        /// get the singleton instance
        static AF_Variable_Storage* get_instance(void) {
            if (_instance == nullptr) {
                _instance = new AF_Variable_Storage();
            }
            return _instance;
        }

        /// @brief stores a new variable
        /// @return true if the variable was added successfully, false if the identifier already exists in the storage
        void add_variable(AF_Variable* var) {
            
            // TODO: check if the identifier already exists

            // add the new node to the tail end of the linked list
            AF_Variable_Node* node = new AF_Variable_Node();
            node->var = var;
            node->next = nullptr;

            // if the linked list is empty, set the head to the new node
            if (_head == nullptr) {
                _head = node;
            } else {
                // add to the tail
                _tail->next = node;
                _tail = node;
            }

            _num_variables++;

        }

    protected:

        /// the number of variables
        uint16_t _num_variables = 0;
        /// the head of the linked list of variables (the first node)
        AF_Variable_Node* _head = nullptr;
        /// the tail of the linked list of variables (the last node)
        AF_Variable_Node* _tail = nullptr;
        
    private:
        /// Private constructor
        AF_Variable_Storage() {} 
        /// the singleton instance
        static AF_Variable_Storage* _instance;
};

class AF_Variable {

    public:
        /// get the type of the variable
        af_var_type get_type(void) const { return _vt; }
        /// get the identifier of the variable
        const char* get_idfr(void) const { return _idfr; }

        // flag readers
        
        /// whether the gcs can change this variable on-the-fly
        bool is_writable_by_gcs(void) const;
        /// whether the gcs can read this variable
        bool is_readable_by_gcs(void) const;
        /// whether this variable is stored in EEPROM
        bool is_eeprom_stored(void) const;
        /// whether this variable is written to the crash-protected SD card
        bool is_blackbox_logged(void) const;

    protected:
        /// the identifier of the variable
        const char * _idfr;
        /// the type of the variable
        af_var_type _vt;
        /// whether to publish this variable to the GCS
        bool _publish_to_gcs;

        uint8_t _flags;

    public:
        /// constructor
        AF_Variable(const char* idfr, af_var_type vt, uint8_t flags) {
            _idfr = idfr;
            _vt = vt;
            _flags = flags;

            // add the variable to the storage
            AF_Variable_Storage::get_instance()->add_variable(this);
        }
        /// get identifier
        const char* get_idfr(void) { return _idfr; }

};

/// template class for scalar variables
template <typename T, af_var_type VT>
class AF_Var_Scalar: public AF_Variable {

    public:
        /// constructor
        AF_Var_Scalar(const char* idfr, const T initial_value, uint8_t flags): AF_Variable(idfr, VT, flags) {
            val = initial_value;
        }

        /// get value
        const T& get() const { return _val; }

        /// set value
        void set(const T& val) {
            if (_publish_to_gcs) {
                // TODO: tell GCS that the variable updated

            }
            _val = val;
        }

        /// cast to T
        operator const T &() const {
            return _value;
        }

        /// assignment operator
        AF_Var_Scalar<T, VT> &operator=(const T& new_val) {
            set(new_val);
            return *this;
        }
        
    private:
        static const af_var_type _vt = VT;

    protected:
        /// the value of the variable
        T _val;

    public:
        
};

// define common types

AF_DEF_VARTYPE_SCALAR(bool, Bool, AF_VAR_BOOL);    // AF_Bool
AF_DEF_VARTYPE_SCALAR(float, Float, AF_VAR_FLOAT);    // AF_Float
AF_DEF_VARTYPE_SCALAR(uint8_t, UInt8, AF_VAR_UINT8);  // AF_UInt8
AF_DEF_VARTYPE_SCALAR(int8_t, Int8, AF_VAR_INT8);     // AF_Int8
AF_DEF_VARTYPE_SCALAR(uint16_t, UInt16, AF_VAR_UINT16);  // AF_UInt16
AF_DEF_VARTYPE_SCALAR(int16_t, Int16, AF_VAR_INT16);  // AF_Int16
AF_DEF_VARTYPE_SCALAR(uint32_t, UInt32, AF_VAR_UINT32);  // AF_UInt32
AF_DEF_VARTYPE_SCALAR(int32_t, Int32, AF_VAR_INT32);  // AF_Int32

#endif