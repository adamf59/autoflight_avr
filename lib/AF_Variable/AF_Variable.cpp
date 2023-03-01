#include "AF_Variable.h"

#include <lib/AF_Logger/AF_Logger.h>

AF_Variable_Storage* AF_Variable_Storage::_instance;

bool AF_Variable::is_readable_by_gcs(void) const {
    return _flags & AF_VAR_FLAG_READABLE_BY_GCS;
}

bool AF_Variable::is_writable_by_gcs(void) const {
    return is_readable_by_gcs() && (_flags & AF_VAR_FLAG_WRITABLE_BY_GCS);
}

bool AF_Variable::is_eeprom_stored(void) const {
    return _flags & AF_VAR_FLAG_EEPROM_STORED;
}

bool AF_Variable::is_blackbox_logged(void) const {
    return _flags & AF_VAR_FLAG_BLACKBOX_LOGGED;
}
