CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(vecter)

include(CTest)

set(CHCK_POOL_DIR lib/chck/pool)
set(LIBFIXMATH_DIR lib/libfixmath/libfixmath)
set(LIBFIXMATRIX_DIR lib/libfixmatrix)

set(VECTER_INCLUDE_DIRS
  ${vecter_SOURCE_DIR}/include
  ${vecter_SOURCE_DIR}/${LIBFIXMATH_DIR}
  ${vecter_SOURCE_DIR}/${LIBFIXMATRIX_DIR}
)

include_directories(
  include
  ${CHCK_POOL_DIR}
  ${LIBFIXMATH_DIR}
  ${LIBFIXMATRIX_DIR}
)

file(GLOB SOURCES 
  src/*.c
  ${CHCK_POOL_DIR}/*.c
  ${LIBFIXMATH_DIR}/*.c
  ${LIBFIXMATRIX_DIR}/fixarray.c
  ${LIBFIXMATRIX_DIR}/fixmatrix.c
  ${LIBFIXMATRIX_DIR}/fixquat.c
  ${LIBFIXMATRIX_DIR}/fixstring.c
  ${LIBFIXMATRIX_DIR}/fixvector2d.c
  ${LIBFIXMATRIX_DIR}/fixvector3d.c
)

list(APPEND CMAKE_C_FLAGS "-Wall -Wextra")

add_library(vecter
    ${SOURCES}
)

add_subdirectory(test)
add_subdirectory(example)
