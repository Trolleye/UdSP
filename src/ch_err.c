#include "../include/ch_err.h"

const char *ch_err_str(ch_err err)
{
    switch (err) {
    case CH_OK:         return "ok";
    case CH_ERR_ARG:    return "neplatny argument";
    case CH_ERR_MEM:    return "nedostatok pamate";
    case CH_ERR_RANGE:  return "hodnota mimo rozsahu";
    case CH_ERR_FORMAT: return "neplatny format vstupu";
    case CH_ERR_IO:     return "chyba vstupu alebo vystupu";
    case CH_ERR_NOT_IMPLEMENTED: return "funkcia nie je implementovana";
    }
    return "neznama chyba";
}
