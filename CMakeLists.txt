# CMakeLists.txt para GLClock
cmake_minimum_required(VERSION 3.10)
project(GLClock)

# Opciones de compilación
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

# Archivos fuente y encabezados
set(SOURCES
    main.cpp
    glclock.cpp
    GLObject.cpp
    GLextensions.cpp
    PnmIO.cpp
    MString.cpp
)

set(HEADERS
    glclock.H
    GLObject.H
    GLextensions.H
    Option.H
    PnmIO.H
    MString.H
    Define.H
    Math3D.H
)

# Binarios que se generarán
add_executable(glclock ${SOURCES})
add_executable(glclockmark glclockmark.cpp MString.cpp)

# Incluye rutas
include_directories(/usr/X11R6/include)

# Librerías requeridas
set(LIBS -lglut -lGLU -lGL -lXi -lXmu -lXext -lX11 -lm)
set(MARK_LIBS -lm)

# Enlace de bibliotecas
target_link_libraries(glclock ${LIBS})
target_link_libraries(glclockmark ${MARK_LIBS})

# Limpieza: cambiar "clean" a "clean_all" para evitar conflictos con nombres reservados
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E rm -f *.o glclock glclockmark *~ #*#
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
