#ifndef H_SHADER_MANAGER
#define H_SHADER_MANAGER

#include "common.h"

// H_LOGGER linkage

extern my_errno
logger_log_shader_info(GLuint shader);

extern my_errno
logger_log_program_info(GLuint program);

#endif // H_SHADER_MANAGER
