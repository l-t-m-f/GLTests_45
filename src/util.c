#include "util.h"

my_errno util_load_file_raw(const char *path, char **result);
my_errno util_update_fps_counter(GLFWwindow *window);
my_errno util_GLtype_to_string(GLenum type, const char **enum_result);

my_errno util_GLtype_to_string(GLenum type, const char **enum_result)
{
    my_errno result = 0;
    switch (type)
    {
    case GL_BOOL:
        *enum_result = "bool";
    case GL_INT:
        *enum_result = "int";
    case GL_FLOAT:
        *enum_result = "float";
    case GL_FLOAT_VEC2:
        *enum_result = "vec2";
    case GL_FLOAT_VEC3:
        *enum_result = "vec3";
    case GL_FLOAT_VEC4:
        *enum_result = "vec4";
    case GL_FLOAT_MAT2:
        *enum_result = "mat2";
    case GL_FLOAT_MAT3:
        *enum_result = "mat3";
    case GL_FLOAT_MAT4:
        *enum_result = "mat4";
    case GL_SAMPLER_2D:
        *enum_result = "sampler2D";
    case GL_SAMPLER_3D:
        *enum_result = "sampler3D";
    case GL_SAMPLER_CUBE:
        *enum_result = "samplerCube";
    case GL_SAMPLER_2D_SHADOW:
        *enum_result = "sampler2DShadow";
    default:
        GOTO_END(-1);
        break;
    }
ret:
    return result;
}

my_errno util_load_file_raw(const char *path, char **content_result)
{
    my_errno result = 0;

    FILE *fp = NULL;
    size_t file_content_length = 0;

    fp = fopen(path, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error! There was a problem loading file '%s'!", path);
        GOTO_END(-1);
    }

    fseek(fp, 0L, SEEK_END);
    file_content_length = (unsigned long long)ftell(fp) + 1;

    fclose(fp);

    if (file_content_length == 0)
    {
        fprintf(stderr, "Error! file content of %s was length 0.", path);
        GOTO_END(-1);
    }

    *content_result = calloc(file_content_length, sizeof(char));

    fp = NULL;

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error! There was a problem loading file '%s'!",
                path);
        GOTO_END(-1);
    }

    fread(*content_result, 1, file_content_length - 1, fp);

    fclose(fp);

ret:
    return result;
}

my_errno util_update_fps_counter(GLFWwindow *window)
{
    my_errno result = 0;
    static double previous_seconds = 0.0;
    static int frame_count = 0;

    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;
    frame_count++;

    if (elapsed_seconds >= 1.0)
    {
        previous_seconds = current_seconds;
        double fps = (double)frame_count / elapsed_seconds;
        char tmp[128];
        sprintf(tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    return result;
}
