cmake_minimum_required(VERSION 2.8.11)
project(qmaze)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools)

set(QMaze_SRCS
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
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lang/lang.qrc.in ${CMAKE_CURRENT_BINARY_DIR}/lang.qrc)
  qt5_add_resources(QM_RC ${CMAKE_CURRENT_BINARY_DIR}/lang.qrc)
endif(${Qt5LinguistTools_FOUND})

add_executable(${PROJECT_NAME} ${QMaze_SRCS} ${QM_RC} Cliparts.qrc)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_PACKAGE_NAME "QMaze")
set(CPACK_PACKAGE_CONTACT "Daniel Steinhauer <d.steinhauer@mailbox.org>")
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 1)
set(CPACK_PACKAGE_VERSION_PATCH 1)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple maze game.")
set(CPACK_PACKAGE_DESCRIPTION "This Qt based game creates mazes the player can go through. But he/she needs to collect the key first to finish a level.")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libgcc1, libstdc++6, libqt5core5a, libqt5gui5, libqt5widgets5")

set(CPACK_GENERATOR "DEB")
include(CPack)