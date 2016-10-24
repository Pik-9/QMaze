cmake_minimum_required(VERSION 2.8.11)
project(qmaze)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools)

set(QMemory_SRCS
  src/QMazeWindow.cpp
  src/maze.cpp
  src/random.cpp
  src/main.cpp
)

set(QMaze_Lang
  lang/QMaze_de.ts
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(${Qt5LinguistTools_FOUND})
  qt5_add_translation(QM ${QMaze_Lang})
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lang/lang.qrc ${CMAKE_CURRENT_BINARY_DIR}/lang.qrc)
  qt5_add_resources(QM_RC ${CMAKE_CURRENT_BINARY_DIR}/lang.qrc)
  message(${QM_RC})
endif(${Qt5LinguistTools_FOUND})

add_executable(${PROJECT_NAME} ${QMemory_SRCS} ${QM_RC} Cliparts.qrc)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)