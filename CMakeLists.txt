cmake_minimum_required(VERSION 3.1)
project(qmaze)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools)

set(QMaze_SRCS
  src/QMazeWindow.cpp
  src/maze.cpp
  src/main.cpp
)

set(QMaze_Lang
  lang/QMaze_de.ts
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)

option(DEP_PKG OFF)
option(RPM_PKG OFF)

if(${Qt5LinguistTools_FOUND})
  qt5_add_translation(QM ${QMaze_Lang})
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lang/lang.qrc.in ${CMAKE_CURRENT_BINARY_DIR}/lang.qrc)
  qt5_add_resources(QM_RC ${CMAKE_CURRENT_BINARY_DIR}/lang.qrc)
endif(${Qt5LinguistTools_FOUND})

add_executable(${PROJECT_NAME} ${QMaze_SRCS} ${QM_RC} Cliparts.qrc)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  install(FILES res/Icon.png DESTINATION share/QMaze)
  install(PROGRAMS qmaze.desktop DESTINATION /usr/share/applications)
endif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

if(${DEB_PKG})
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libgcc1, libstdc++6, libqt5core5a, libqt5gui5, libqt5widgets5")
  set(CPACK_GENERATOR "DEB")
endif(${DEB_PKG})
if(${RPM_PKG})
  set(CPACK_RPM_PACKAGE_REQUIRES "libQt5Widgets5, libQt5Gui5, libQt5Core5, glibc")
  set(CPACK_GENERATOR "RPM")
endif(${RPM_PKG})
if((${DEB_PKG}) OR (${RPM_PKG}))
  set(CPACK_PACKAGE_NAME "QMaze")
  set(CPACK_PACKAGE_CONTACT "Daniel Steinhauer <d.steinhauer@mailbox.org>")
  set(CPACK_PACKAGE_VERSION_MAJOR 1)
  set(CPACK_PACKAGE_VERSION_MINOR 2)
  set(CPACK_PACKAGE_VERSION_PATCH 1)
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple maze game.")
  set(CPACK_PACKAGE_DESCRIPTION "This Qt based game creates mazes the player can go through. But he/she needs to collect the key first to finish a level.")
  set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
  include(CPack)
endif((${DEB_PKG}) OR (${RPM_PKG}))