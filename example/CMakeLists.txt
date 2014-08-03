CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(vecter-examples)

find_package(PkgConfig REQUIRED)
pkg_check_modules(sdl2 REQUIRED sdl2)

include_directories(
  ${VECTER_INCLUDE_DIRS}
  ${sdl2_INCLUDE_DIRS}
)

add_executable(mouse-follow mouse-follow.c)
target_link_libraries(mouse-follow vecter ${sdl2_LIBRARIES})

