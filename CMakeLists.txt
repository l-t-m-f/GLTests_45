cmake_minimum_required(VERSION 3.24)
project(OPEN_GL_4_5_TESTS C)

set(CMAKE_C_STANDARD 99)

file(GLOB SRC "src/*.c")
file(GLOB INCLUDE_DIR "include")
file(GLOB SRC_HEADERS "headers")

set(CMAKE_MY_C_FLAGS "-Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wwrite-strings -Waggregate-return")
set(CMAKE_STRICT_C_FLAGS "-Wstrict-prototypes -Wstrict-overflow=2")
set(CMAKE_CAST_C_FLAGS "-Wcast-align -Wcast-qual -Wconversion")
set(CMAKE_SWITCH_C_FLAGS "-Wswitch-default -Wswitch-enum")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${CMAKE_MY_C_FLAGS} ${CMAKE_SWITCH_C_FLAGS} ${CMAKE_STRICT_C_FLAGS} ${CMAKE_CAST_C_FLAGS} -Wunreachable-code" CACHE STRING "C Flags" FORCE)

add_executable(${PROJECT_NAME} ${SRC})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dat DESTINATION ${CMAKE_BINARY_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${SRC_HEADERS})
target_link_libraries(${PROJECT_NAME} -lopengl32 -lglew32 -lglfw3 -lm)
