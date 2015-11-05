cmake_minimum_required(VERSION 2.8)

set (CMAKE_BUILD_TYPE debug)

project(timeWidget)

find_package(VTK REQUIRED)
find_package(Qt4 REQUIRED)	

include(${VTK_USE_FILE})
include(${QT_USE_FILE})

add_executable(timeWidget MACOSX_BUNDLE
		TimerWidget.cpp
		main.cpp)

target_link_libraries(timeWidget ${VTK_LIBRARIES} ${QT_LIBRARIES})
