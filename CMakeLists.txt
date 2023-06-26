cmake_minimum_required(VERSION 3.21)
project(mixeur-musique)
set(CMAKE_CXX_STANDARD 14)


# Some directories
set(bin ${CMAKE_SOURCE_DIR}/bin)
set(data ${CMAKE_SOURCE_DIR}/data)
file(MAKE_DIRECTORY ${bin})
file(MAKE_DIRECTORY ${data})


# Executable
set(exe_name mixeur-musique)
add_executable(${exe_name}
        src/main.cpp
        src/model/Playlist.h
        src/model/Playlist.cpp
        )

target_link_libraries(${exe_name} -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio)
set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin})
