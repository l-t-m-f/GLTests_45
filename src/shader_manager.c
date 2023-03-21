#include "shader_manager.h"

/* Implementation of m-dict begins */

#include "m-string.h"
#include "m-dict.h"

static bool is_valid(GLuint GLprogram);

static inline bool oor_equal_p(unsigned k, unsigned char n)
{
  return k == (unsigned)(-n - 1);
}

static inline void oor_set(unsigned *k, unsigned char n)
{
  *k = (unsigned)(-n - 1);
}

DICT_OA_DEF2(dict_unsigned,
             unsigned,
             M_OPEXTEND(M_BASIC_OPLIST,
                        OOR_EQUAL(oor_equal_p),
                        OOR_SET(API_2(oor_set))),
             unsigned,
             M_BASIC_OPLIST)

/* Implementation of m-dict ends */

typedef enum ShaderManagerBitflagName
{
  HAS_VERTEX_SHADER_FLAG = 0x1,
  HAS_FRAGMENT_SHADER_FLAG = 0x2,
  HAS_GEOMETRY_SHADER_FLAG = 0x4,
  HAS_TESSELLATION_SHADER_FLAG = 0x8
} ShaderManagerBitflagNameType;

/**
 * ShaderManager
 * Just three dictionaries (m-dict.h) for now.
 */
typedef struct ShaderManager
{
  byte loaded_shaders;
  dict_unsigned_t vertex_shaders;
  dict_unsigned_t fragment_shaders;
  dict_unsigned_t geometry_shaders;
  dict_unsigned_t tessellation_shaders;
  dict_unsigned_t programs;
} ShaderManager;

/**
 * Initializes a void pointer to be act as a ShaderManager.
 */
my_errno shader_manager_init(void **sm)
{
  // Allocate memory for the shader manager
  *sm = (ShaderManager *)malloc(sizeof(ShaderManager));
  ShaderManager *shader_manager = (ShaderManager *)(*sm);
  if (shader_manager == NULL)
  {
    return -1;
  }
  // Initialize the shader manager
  shader_manager->loaded_shaders = 0x0;
  dict_unsigned_init(shader_manager->programs);
  dict_unsigned_init(shader_manager->vertex_shaders);
  dict_unsigned_init(shader_manager->fragment_shaders);
  dict_unsigned_init(shader_manager->geometry_shaders);
  dict_unsigned_init(shader_manager->tessellation_shaders);
  return 0;
}

/**
 * Requests a void pointer already initialized to act as ShaderManager to create a vertex shader under a specified name. The source code must be provided.
 */
my_errno shader_manager_create_vertex_shader(void **sm, const GLchar *shader_source_code, VertexShaderNameType shader_name)
{
  ShaderManager *shader_manager = (ShaderManager *)(*sm);
  if (shader_manager == NULL)
  {
    return -1;
  }

  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertex_shader_id, 1, &shader_source_code, NULL);

  glCompileShader(vertex_shader_id);

  // check for errors
  int compilation_params = -1;
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &compilation_params);

  if (GL_TRUE != compilation_params)
  {
    fprintf(stderr, "Error! GL Shader index %i did not compile\n", vertex_shader_id);
    logger_log_shader_info(vertex_shader_id);
    return -1;
  }

  dict_unsigned_set_at(shader_manager->vertex_shaders, (unsigned int)shader_name, vertex_shader_id);
  return 0;
}

/**
 * Requests a void pointer already initialized to act as ShaderManager to create a fragment shader under a specified name. The source code must be provided.
 */
my_errno shader_manager_create_fragment_shader(void **sm, const GLchar *shader_source_code, FragmentShaderNameType shader_name)
{
  ShaderManager *shader_manager = (ShaderManager *)(*sm);
  if (shader_manager == NULL)
  {
    return -1;
  }

  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragment_shader_id, 1, &shader_source_code, NULL);

  glCompileShader(fragment_shader_id);

  // check for errors
  int compilation_params = -1;
  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &compilation_params);

  if (GL_TRUE != compilation_params)
  {
    fprintf(stderr, "Error! GL Shader index %i did not compile\n", fragment_shader_id);
    logger_log_shader_info(fragment_shader_id);
    return -1;
  }

  dict_unsigned_set_at(shader_manager->fragment_shaders, (unsigned int)shader_name, fragment_shader_id);
  return 0;
}

/**
 * Requests a void pointer already initialized to act as ShaderManager to create a new shader GLprogram with a specified vertex and fragment shader.
 */
my_errno shader_manager_create_GLprogram(void **sm, GLProgramNameType GLprogram_name, VertexShaderNameType vertex_shader, FragmentShaderNameType fragment_shader)
{
  ShaderManager *shader_manager = (ShaderManager *)(*sm);
  if (shader_manager == NULL)
  {
    return -1;
  }

  GLuint GLprogram = glCreateProgram();

  if (vertex_shader != VS_NONE)
  {
    glAttachShader(GLprogram, *dict_unsigned_get(shader_manager->fragment_shaders, (unsigned int)fragment_shader));

    shader_manager->loaded_shaders |= HAS_VERTEX_SHADER_FLAG;
  }

  if (fragment_shader != FS_NONE)
  {
    glAttachShader(GLprogram, *dict_unsigned_get(shader_manager->vertex_shaders, (unsigned int)vertex_shader));

    shader_manager->loaded_shaders |= HAS_FRAGMENT_SHADER_FLAG;
  }

  glLinkProgram(GLprogram);

  int link_params = -1;
  glGetProgramiv(GLprogram, GL_LINK_STATUS, &link_params);
  if (GL_TRUE != link_params)
  {
    fprintf(stderr,
            "Error! Could not link shader GLprogram GL index %u \n",
            GLprogram);
    logger_log_program_info(GLprogram);
    return -1;
  }
  dict_unsigned_set_at(shader_manager->programs, (unsigned int)GLprogram_name, GLprogram);
  return 0;
}

/**
 * Get a shader GLprogram by name. It is returned by its ID in result_program.
 */
my_errno shader_manager_get_program(void *sm, GLProgramNameType GLprogram, GLuint *result_program)
{
  ShaderManager *shader_manager = (ShaderManager *)sm;
  if (shader_manager == NULL)
  {
    return -1;
  }
  *result_program = *dict_unsigned_get(shader_manager->programs, (unsigned int)GLprogram);

  return 0;
}

my_errno shader_manager_set_uniform(void **sm, GLProgramNameType GLprogram, const GLchar *uniform_field_name)
{
  printf("Function is not implemented\n");
  return 0;
}

/**
 * Only do this during development, because it is quite computationally expensive.
 * @param GLprogram
 * @return
 */
bool is_valid(GLuint GLprogram)
{
  glValidateProgram(GLprogram);
  int params = -1;
  glGetProgramiv(GLprogram, GL_VALIDATE_STATUS, &params);
  printf("GLprogram %i GL_VALIDATE_STATUS = %i \n",
         GLprogram,
         params);
  if (GL_TRUE != params)
  {
    logger_log_program_info(GLprogram);
    return false;
  }
  return true;
}
