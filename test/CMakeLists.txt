CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(vecter-tests)

include_directories(
  ${VECTER_INCLUDE_DIRS}
)

add_executable(trivial-move trivial-move.c)
target_link_libraries(trivial-move vecter)
add_test(trivial-move trivial-move)

add_executable(trivial-wall trivial-wall.c)
target_link_libraries(trivial-wall vecter)
add_test(trivial-wall trivial-wall)

add_executable(sharp-angle sharp-angle.c)
target_link_libraries(sharp-angle vecter)
add_test(sharp-angle sharp-angle)

add_executable(collision-order collision-order.c)
target_link_libraries(collision-order vecter)
add_test(collision-order collision-order)

add_executable(wall-push wall-push.c)
target_link_libraries(wall-push vecter)
add_test(wall-push wall-push)
