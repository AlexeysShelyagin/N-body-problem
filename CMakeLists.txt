cmake_minimum_required(VERSION 3.20)
project(N_body_problem)

set(CMAKE_CXX_STANDARD 17)

add_executable(N_body_problem main.cpp Vectors.cpp Vectors.h Entities.h Entities.cpp Physics.cpp Loader.cpp Loader.h Config.h Renderer.h Renderer.cpp Physics.h UI.h UI.cpp Files.cpp Files.h Replay.cpp Replay.h)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/Libraries/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/Libraries/SFML/include)
target_link_libraries(N_body_problem sfml-system sfml-window sfml-graphics)

#target_link_libraries(N_body_problem -lOpenGL32 -lFreeGLUT)