#include "errorsHandler.hpp"

const char* getErrorMessage(Errors error) {
    #define ERROR_UNPACK(errName, errCode, errMessage) \
        case errName##_ERROR:\
            return "Error: " errMessage ".\n";

    //LOG_DEBUG_VARS(error, error == ACCESS_TO_EMPTY_STACK_ERROR);
    switch (error) {
        case STATUS_OK:
            return "No errors occured.\n";
        #include "errorsPlainText.txt"
        default:
            return "Error: unknown error.\n";
    }

    #undef ERROR_UNPACK
}
