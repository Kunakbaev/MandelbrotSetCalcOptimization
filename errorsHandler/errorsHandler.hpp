#ifndef LAB1STACKS_ERRORS_HANDLER_ERRORS_HANDLER_HPP
#define LAB1STACKS_ERRORS_HANDLER_ERRORS_HANDLER_HPP

#include "../external/LoggerLib/include/logLib.hpp"

#define ERROR_UNPACK(errName, errCode, errMessage)                      \
    errName##_ERROR = errCode,

enum Errors {
    STATUS_OK = 0,
    #include "errorsPlainText.txt"
};

#undef ERROR_UNPACK

const char* getErrorMessage(Errors error);








#define IF_ARG_NULL_RETURN(arg)                                         \
    do {                                                                \
        auto tmp = arg;                                                 \
        if (tmp == NULL) {                                              \
            LOG_ERROR(getErrorMessage(INVALID_ARGUMENT_ERROR));         \
            assert(#arg && tmp != NULL);                                \
            return INVALID_ARGUMENT_ERROR;                              \
        }                                                               \
    } while (0)

#define IF_ERR_RETURN(error)                                            \
    do {                                                                \
        Errors errorTmp = error;                                        \
        if (errorTmp != STATUS_OK) {                                    \
            LOG_ERROR(getErrorMessage(errorTmp));                       \
            assert(errorTmp != STATUS_OK);                              \
            return INVALID_ARGUMENT_ERROR;                              \
        }                                                               \
    } while (0)

#define IF_NOT_CONDITION_RETURN(condition, error)                       \
    do {                                                                \
        bool tmpCondition = condition;                                  \
        if (!tmpCondition) {                                            \
            LOG_ERROR(getErrorMessage(error));                          \
            assert(#condition && tmpCondition);                         \
            return error;                                               \
        }                                                               \
    } while (0)

#endif
