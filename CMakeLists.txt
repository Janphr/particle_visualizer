cmake_minimum_required(VERSION 3.17)
project(particle_visualizer)

set(CMAKE_CXX_STANDARD 14)
set(SFML_INCLUDE_DIR "~/installation/SFML/include")
set(SFML_LIBRARY_DIR "~/installation/SFML/lib")
set(SFML_DIR "~/installation/SFML/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)
find_package(OpenGL REQUIRED)
find_package(OpenMP)
find_package(OpenCV 3.4.2 REQUIRED PATHS "~/installation/OpenCV-3.4.2/share/OpenCV/")

if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()

if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

link_directories(SFML_LIBRARY_DIR)
include_directories(include
        SFML_INCLUDE_DIR
        ${OpenCV_INCLUDE_DIRS})

add_executable(particle_visualizer
        src/main.cpp
        src/particle.cpp
        src/particle_system.cpp
        src/render_engine.cpp
        include/worker.h)

target_link_libraries (particle_visualizer sfml-graphics sfml-window sfml-system OpenGL::GL ${OpenCV_LIBS})

