#include "main.h"

void *shader_manager = NULL;

void glfw_error_callback(int error, const char *description);
void glfw_window_size_callback(GLFWwindow *window, int width, int height);

int g_gl_width = 640;
int g_gl_height = 480;

const char *vert_filename = "dat/test.vert";
const char *frag_filename = "dat/test.frag";
const char *frag2_filename = "dat/test2.frag";

int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        argv[i] = NULL;
    }
    assert(logger_restart() > -1);

    shader_manager_init(&shader_manager);

    logger_stand("Starting GLFW\n%s\n", glfwGetVersionString());
    glfwSetErrorCallback(glfw_error_callback);

    if (glfwInit() == false)
    {
        fprintf(stderr, "Error! Could not start GLFW3.\n");
        return 1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWmonitor *mon = glfwGetPrimaryMonitor();
    const GLFWvidmode *vmode = glfwGetVideoMode(mon);

    GLFWwindow *window = glfwCreateWindow(g_gl_width,
                                          g_gl_height,
                                          "Extended GL Init",
                                          NULL,
                                          NULL);
    if (window == false)
    {
        fprintf(stderr, "Error! Could not open window with GLFW3.\n");
        glfwTerminate();
        return 1;
    }

    glfwSetWindowSizeCallback(window, glfw_window_size_callback);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    logger_get_gl_params();

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported: %s\n", version);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /* clang-format off */
  GLfloat points[] = {
      -.5f, .66f, 0.f,
      .5f, -.66f, 0.f,
      -.5f, -.66f, 0.f,
      .5f, -.66f, 0.f,
      .5f, .66f, 0.f,
      -.5f, .66f, 0.f,

      -1.f, -1.f, 0.f,
      -1.f, 0.f, 0.f,
      -.5f, -.25f, 0.f,
  };
    /* clang-format on */

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint vao[2] = {0};
    glGenVertexArrays(2, vao);

    glBindVertexArray(vao[0]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Tells OpenGL that the array is divided into variables of 3 floats each
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(vao[1]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Prepare programs
    const char *vert_shader = NULL;
    util_load_file_raw(vert_filename, &vert_shader);
    shader_manager_create_vertex_shader(&shader_manager, vert_shader,
                                        VS_TEST1);

    const char *frag_shader = NULL;
    util_load_file_raw(frag_filename, &frag_shader);
    shader_manager_create_fragment_shader(&shader_manager, frag_shader,
                                          FS_TEST1);

    const char *frag_shader2 = NULL;
    util_load_file_raw(frag2_filename, &frag_shader2);
    shader_manager_create_fragment_shader(&shader_manager, frag_shader2,
                                          FS_TEST2);

    shader_manager_create_GLprogram(&shader_manager, TEST1, VS_TEST1,
                                    FS_TEST1);

    shader_manager_create_GLprogram(&shader_manager, TEST2, VS_TEST1,
                                    FS_TEST2);

    glClearColor(.5f, .6f, .7f, 1.f);

    // Render loop

    while (glfwWindowShouldClose(window) == false)
    {
        GLuint loaded_GLprogram = 0;

        util_update_fps_counter(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_manager_get_program(shader_manager, TEST1, &loaded_GLprogram);
        glUseProgram(loaded_GLprogram);

        const GLfloat values[] = {1.f, 1.f, 0.f, 1.f};
        glUniform4fv(glGetUniformLocation(loaded_GLprogram, "input_color"),
                     1,
                     values);

        glBindVertexArray(vao[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        shader_manager_get_program(shader_manager, TEST2, &loaded_GLprogram);
        glUseProgram(loaded_GLprogram);

        glBindVertexArray(vao[1]);
        glDrawArrays(GL_TRIANGLES, 6, 9);
        glfwPollEvents();
        glfwSwapBuffers(window);
        glViewport(0, 0, g_gl_width, g_gl_height);

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, 1);
        }
    }

    glfwTerminate();

    return 0;
}

void glfw_error_callback(int error, const char *description)
{
    logger_error("GLFW Error! Code %i msg: %s\n", error, description);
}

void glfw_window_size_callback(GLFWwindow *window, int width, int height)
{
    g_gl_width = width;
    g_gl_height = height;
}