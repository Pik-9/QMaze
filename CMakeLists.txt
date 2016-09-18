cmake_minimum_required(VERSION 2.6)
project(qmemory)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(QMemory_SRCS QMemory.cpp main.cpp)

qt4_automoc(${QMemory_SRCS})
add_executable(qmemory ${QMemory_SRCS})
target_link_libraries(qmemory ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS qmemory RUNTIME DESTINATION bin)
