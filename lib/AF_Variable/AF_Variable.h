#ifndef AF_VARIABLE_H_
#define AF_VARIABLE_H_

/// @file   AF_Variable.h
/// @brief  standardizes types for AutoFlight, and exposes an interface for building
///         on top of an AP_Variable, such as auto-storing the variable in EEPROM.
///         Exposes declared variables to the GCS in packets.

#define NDEBUG

#include <stdlib.h>
#include <stdint.h>

/// max length of an `AP_Variable` identifier
#define AF_VAR_MAX_IDFR_LEN 16

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

/// management class for variables

class AF_Variable_Storage {

    public:
        
        /// get the number of variables
        uint16_t get_num_variables(void) const { return _num_variables; }

        /// get the variable with the given identifier
        // AF_Variable* get_variable(af_var_idfr_t idfr) const { return _variables.at(idfr); }

        /// get the singleton instance
        static AF_Variable_Storage* get_instance(void) {
            if (_instance == nullptr) {
                _instance = new AF_Variable_Storage();
            }
            return _instance;
        }

    protected:

        /// the number of variables
        uint16_t _num_variables = 0;
        /// mapping of variable id -> variable id's pointer
        // std::map<af_var_idfr_t, AF_Variable*> _variables;
    
    private:
        /// the singleton instance
        static AF_Variable_Storage* _instance;


    public:
        /// constructor
        AF_Variable_Storage(void) {
            // do nothing
        }
        /// add a variable
        /// @return true if the variable was added successfully, false if the identifier already exists in the storage
        bool add_variable(AF_Variable* var) {
            
            // check if the variable already exists
            // if (_variables.find(var->get_idfr()) != _variables.end()) {
                // variable already exists
                // return false;
            // }
            
            /// put the varaible in the map
            // _variables[var->get_idfr()] = var;

        }

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


/// macro for typedefining instances of the AF_VarScalar template.
/// _t is the base type
/// _name will be the name of the typedef, with AF_ prepended
/// _vt is the enum af_var_type type
#define AF_DEF_VARTYPE_SCALAR(_t, _name, _vt)  typedef AF_Var_Scalar<_t, _vt> AF_ ## _name;


// define common types
AF_DEF_VARTYPE_SCALAR(bool, Bool, AF_VAR_BOOL);    // AF_Float
AF_DEF_VARTYPE_SCALAR(float, Float, AF_VAR_FLOAT);    // AF_Float
AF_DEF_VARTYPE_SCALAR(int8_t, Int8, AF_VAR_INT8);     // AF_Int8
AF_DEF_VARTYPE_SCALAR(int16_t, Int16, AF_VAR_INT16);  // AF_Int16
AF_DEF_VARTYPE_SCALAR(int32_t, Int32, AF_VAR_INT32);  // AF_Int32
AF_DEF_VARTYPE_SCALAR(uint16_t, UInt16, AF_VAR_UINT16);  // AF_UInt16

#endif