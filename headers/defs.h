#ifndef H_DEFS
#define H_DEFS

typedef enum GLProgramName
{
  NONE = -1,
  TEST1 = 0,
  TEST2,
} GLProgramNameType;

typedef enum VertexShaderName
{
  VS_NONE = -1,
  VS_TEST1 = 0,
  VS_TEST2,
} VertexShaderNameType;

typedef enum FragmentShaderName
{
  FS_NONE = -1,
  FS_TEST1 = 0,
  FS_TEST2,
} FragmentShaderNameType;

typedef signed char my_errno;

typedef char byte;

#define GOTO_END(x) \
  result = x;       \
  goto ret

#endif // H_DEFS