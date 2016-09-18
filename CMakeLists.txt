cmake_minimum_required(VERSION 2.8.11)
project(qmemory)

find_package(Qt5Widgets REQUIRED)
set(QMemory_SRCS
  src/QMemory.cpp
  src/maze.cpp
  src/random.cpp
  src/main.cpp
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME} ${QMemory_SRCS})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

install(TARGETS qmemory RUNTIME DESTINATION bin)