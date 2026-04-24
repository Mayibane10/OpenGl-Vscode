set(projName LearnOpenGl)

cmake_minimum_required(VERSION 3.10.0)
project(${projName} VERSION 0.1.0 LANGUAGES C CXX)

# === Library directories ===
set(GLFW_DIR libraries/GLFW-x64)
set(GLAD_DIR libraries/GLAD-x86_64)
set(GLM_DIR  libraries/GLM)
set(GLEW_x64 libraries/GLEW-x64)

# === Include paths ===
include_directories(
    ${GLFW_DIR}/include
    ${GLAD_DIR}/include
    ${GLM_DIR}/
    ${GLEW_x64}/include

    src/ # If used check the Source folre
    / # Root folder (always needed)
    
)

# === Link GLFW lib folder ===
link_directories(${GLFW_DIR}/lib-mingw-w64)

# === Link GLEW lib folder ===
link_directories(${GLEW_x64}/lib/Release/x64)   # adjust path to match your folder

# === Add GLAD ===
add_library(glad STATIC "${GLAD_DIR}/src/glad.c")

# === Executable ===
add_executable(${PROJECT_NAME} src/main.cpp)

# === Link everything ===
target_link_libraries(${PROJECT_NAME}
    glad
    glfw3
    glew32
    opengl32

)

# after compiling...
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD

# copy the .dll to the buld folder
COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/${GLEW_x64}/bin/Release/x64/glew32.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>

COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/${GLFW_DIR}/lib-mingw-w64/glfw3.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>

# Copy shaders folder next to the executable after build
COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)
