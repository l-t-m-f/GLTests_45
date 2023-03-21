#ifndef H_MAIN
#define H_MAIN

#include "common.h"

// H_LOGGER dependencies

extern my_errno
logger_restart(void);

extern my_errno
logger_get_gl_params(void);

extern my_errno
logger_error(const char *message, ...);

extern my_errno
logger_stand(const char *message, ...);

// H_SHADER_MANAGER depencencies

extern void
shader_manager_init(void **shader_manager);

extern my_errno
shader_manager_create_GLprogram(void **sm, GLProgramNameType programme_name, VertexShaderNameType vertex_shader, FragmentShaderNameType fragment_shader);

extern my_errno
shader_manager_get_program(void *sm, GLProgramNameType programme, GLuint *result_programme);

extern my_errno
shader_manager_create_vertex_shader(void **sm, const GLchar *shader_source, VertexShaderNameType shader_name);

extern my_errno
shader_manager_create_fragment_shader(void **sm, const GLchar *shader_source, FragmentShaderNameType shader_name);

// H_UTIL dependencies

extern char *
util_load_file_raw(const char *path, char **result);

extern void
util_update_fps_counter(GLFWwindow *window);

extern const char *
util_GLtype_to_string(GLenum type);

#endif // H_MAIN
