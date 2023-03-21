#include "logger.h"

#define GL_LOG_FILE "gl.log"

my_errno logger_restart(void);
my_errno logger_stand(const char *message, ...);
my_errno logger_error(const char *message, ...);
my_errno logger_get_gl_params(void);
my_errno logger_log_shader_info(GLuint shader);
my_errno logger_log_program_info(GLuint GLprogram);
my_errno logger_print_all_info(GLuint GLprogram);

/**
 * Generates a new GL_LOG_FILE to be written to by subsequent logger functions.
 */
my_errno logger_restart()
{
    my_errno result = 0;
    FILE *fp = fopen(GL_LOG_FILE, "w");
    if (fp == NULL)
    {
        fprintf(stderr,
                "Error! Could not open GL_LOG_FILE log file %s for writing\n",
                GL_LOG_FILE);

        result = -1;
        goto ret;
    }
    time_t now = time(NULL);
    char *date = ctime(&now);
    fprintf(fp, "GL_LOG_FILE log. local time %s\n", date);
    fclose(fp);

ret:
    return result;
}

/**
 * Log something 'standard' (not an error nor warning) in the GL_LOG_FILE previously initialized.
 */
my_errno logger_stand(const char *message, ...)
{
    my_errno result = 0;
    va_list argptr;
    FILE *fp = fopen(GL_LOG_FILE, "a");
    if (fp == NULL)
    {
        fprintf(stderr,
                "Error! Could not open GL_LOG_FILE %s file for appending\n",
                GL_LOG_FILE);

        result = -1;
        goto ret;
    }
    va_start(argptr, message);
    vfprintf(fp, message, argptr);
    va_end(argptr);
    fclose(fp);

ret:
    return result;
}

/**
 * Log something an error in the GL_LOG_FILE previously initialized.
 */
my_errno logger_error(const char *message, ...)
{
    my_errno result = 0;
    va_list argptr;
    FILE *fp = fopen(GL_LOG_FILE, "a");
    if (fp == NULL)
    {
        fprintf(stderr,
                "Error! Could not open GL_LOG_FILE %s file for appending\n",
                GL_LOG_FILE);
        result = -1;
        goto ret;
    }
    va_start(argptr, message);
    vfprintf(fp, message, argptr);
    va_end(argptr);
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);
    fclose(fp);
ret:
    return result;
}

/**
 * Prints out the OPEN_GL parameters. Currently only in the console.
 */
my_errno logger_get_gl_params(void)
{
    my_errno result = 0;
    GLenum params[] =
        {
            GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
            GL_MAX_CUBE_MAP_TEXTURE_SIZE,
            GL_MAX_DRAW_BUFFERS,
            GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
            GL_MAX_TEXTURE_IMAGE_UNITS,
            GL_MAX_TEXTURE_SIZE,
            GL_MAX_VARYING_FLOATS,
            GL_MAX_VERTEX_ATTRIBS,
            GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
            GL_MAX_VERTEX_UNIFORM_COMPONENTS,
            GL_MAX_VIEWPORT_DIMS,
            GL_STEREO,
        };
    const char *names[] = {
        "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
        "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
        "GL_MAX_DRAW_BUFFERS",
        "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
        "GL_MAX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_TEXTURE_SIZE",
        "GL_MAX_VARYING_FLOATS",
        "GL_MAX_VERTEX_ATTRIBS",
        "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
        "GL_MAX_VIEWPORT_DIMS",
        "GL_STEREO",
    };
    logger_stand("GL Context Params:\n");
    char msg[256];

    for (int i = 0; i < 10; i++)
    {
        int v = 0;
        glGetIntegerv(params[i], &v);
        logger_stand("%s %i\n", names[i], v);
    }
    int v[2];
    v[0] = v[1] = 0;
    glGetIntegerv(params[10], v);
    logger_stand("%s %i %i\n", names[10], v[0], v[1]);
    unsigned char s = 0;
    glGetBooleanv(params[11], &s);
    logger_stand("%s %u\n", names[11], (unsigned int)s);
    logger_stand("--------------------------------\n");
ret:
    return result;
}

/** Prints out the result of glGetShaderInfoLog in the console.*/
my_errno logger_log_shader_info(GLuint shader)
{
    my_errno result = 0;

    const int max_length = 2048;
    int length = 0;
    char info_log[max_length];

    glGetShaderInfoLog(shader, max_length, &length, info_log);

    logger_stand("Shader info log for GL index %u:\n %s\n", shader, info_log);
ret:
    return result;
}

/** Prints out the result of glGetProgramInfoLog in the console.*/
my_errno logger_log_program_info(GLuint GLprogram)
{
    my_errno result = 0;

    const int max_length = 2048;
    int length = 0;
    char info_log[max_length];

    glGetProgramInfoLog(GLprogram, max_length, &length, info_log);

    logger_stand("Shader GLprogram info log for GL index %u:\n %s\n", GLprogram,
                 info_log);
ret:
    return result;
}

/** Prints a whole bunch of information about the current shader GLprogram in the console.*/
my_errno logger_print_all_info(GLuint GLprogram)
{
    my_errno result = 0;

    logger_stand("--------------------------\n Shader Program %i info:\n",
                 GLprogram);
    int params = -1;
    glGetProgramiv(GLprogram, GL_LINK_STATUS, &params);
    logger_stand("GL_LINK_STATUS = %i\n", params);

    glGetProgramiv(GLprogram, GL_ATTACHED_SHADERS, &params);
    logger_stand("GL_ATTACHED_SHADERS = %i\n", params);

    glGetProgramiv(GLprogram, GL_ACTIVE_ATTRIBUTES, &params);
    logger_stand("GL_ACTIVE_ATTRIBUTES = %i\n", params);

    for (GLuint i = 0; i < (GLuint)params; i++)
    {
        const int max_length = 64;
        char name[max_length];
        int actual_length = 0;
        int size = 0;
        GLenum type;

        glGetActiveAttrib(GLprogram, i, max_length, &actual_length, &size,
                          &type, name);

        if (size > 1)
        {
            for (int j = 0; j < size; j++)
            {
                char long_name[max_length];
                sprintf(long_name, "%s[%i]", name, j);
                int location =
                    glGetAttribLocation(GLprogram, long_name);

                logger_stand(" %i) type:%s name:%s location:%i\n",
                             i, util_GLtype_to_string(type), name, location);
            }
        }
        else
        {
            int location = glGetAttribLocation(GLprogram, name);

            logger_stand(" %i) type:%s name:%s location:%i\n", i,
                         util_GLtype_to_string(type), name, location);
        }
    }

    glGetProgramiv(GLprogram, GL_ACTIVE_UNIFORMS, &params);

    logger_stand("GL_ACTIVE_UNIFORMS = %i\n", params);

    for (GLuint i = 0; i < (GLuint)params; i++)
    {
        const int buf_size = 64;
        char name[buf_size];
        int length = 0;
        int size = 0;
        GLenum type;

        glGetActiveUniform(GLprogram, i, buf_size, &length, &size, &type, name);
        if (size > 1)
        {
            for (int j = 0; j < size; j++)
            {
                char long_name[buf_size];
                sprintf(long_name, "%s[%i]", name, j);
                int location = glGetUniformLocation(GLprogram, long_name);

                logger_stand("  %i) type:%s name:%s location:%i\n", i,
                             util_GLtype_to_string(type), long_name, location);
            }
        }
        else
        {
            int location = glGetUniformLocation(GLprogram, name);

            logger_stand("  %i) type:%s name:%s location:%i\n", i,
                         util_GLtype_to_string(type), name, location);
        }
    }
    logger_log_program_info(GLprogram);
ret:
    return result;
}