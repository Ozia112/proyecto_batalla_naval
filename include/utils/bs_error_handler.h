#ifndef BS_ERROR_HANDLER_H
#define BS_ERROR_HANDLER_H

#include <stdio.h>
#include <setjmp.h>

typedef enum {
    BS_ERROR_NONE = 0,
    BS_ERROR_MEMORY_ALLOCATION,
    BS_ERROR_NULL_POINTER,
    BS_ERROR_INVALID_PARAMETER,
    BS_ERROR_FILE_NOT_FOUND,
    BS_ERROR_STRDUP_FAILED
} ErrorCode;

extern jmp_buf bs_error_jump_buffer;
extern ErrorCode bs_last_error;

#define try if ((bs_last_error = setjmp(bs_error_jump_buffer)) == BS_ERROR_NONE)

#define catch(error_code) else if (bs_last_error == (error_code))

#define catch_any else

#define throw(error_code, msg) \
    do { \
        bs_error_log(error_code, msg); \
        longjmp(bs_error_jump_buffer, error_code); \
    } while (0)

#define finally // Simbolico para estructura try-catch-finally

void bs_error_log(ErrorCode code, const char* context);

const char* bs_error_get_message(ErrorCode code);

#endif // BS_ERROR_HANDLER_H